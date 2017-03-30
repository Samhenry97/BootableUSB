////////////////////////////////////////////////
// File:        webserver.c                   //
// Author:      Sam Henry                     //
// Date:        March 17, 2017                //
// Class:       CpS 320, Unix Programming     //
// Desc:        A webserver that handles GET  //
//              requests only.                //
////////////////////////////////////////////////

#include <stdio.h>      // Standard I/O functions
#include <stdlib.h>     // Other standard library functions
#include <errno.h>      // Global errno variable
#include <unistd.h>     // Standard system calls
#include <signal.h>     // Signal handling system calls (sigaction(2))
#include <ctype.h>      // isdigit()

#include "eznet.h"      // Custom networking library
#include "utils.h"      // Custom string splitting in C
#include "threadpool.h" // The thread pool implementation
#include "errors.h"     // Random error messages

// GLOBAL: settings structure instance
struct settings g_settings = {
    .bindhost = "localhost",    // Default: listen only on localhost interface
    .bindport = "5000",         // Default: listen on TCP port 5000
    .threadcount = 5,           // Default: 5 worker threads
    .queuesize = 65536,         // Default: 65536 items in queue
    .verbose = false,           // Default: No extra output
};

// GLOBAL: flag indicating when to shut down server
volatile bool server_running = false;

// Parse commandline options and sets g_settings accordingly.
// Returns 0 on success, -1 on false...
int parse_options(int argc, char * const argv[]) {
    int ret = -1; 

    char op;
    while ((op = getopt(argc, argv, "h:p:r:w:q:v")) > -1) {
        switch (op) {
            case 'h':
                g_settings.bindhost = optarg;
                break;
            case 'p':
                g_settings.bindport = optarg;
                break;
            case 'r':
                if(chdir(optarg) != 0) { goto cleanup; }
                break;
            case 'w':
                g_settings.threadcount = atoi(optarg);
                break;
            case 'v':
                g_settings.verbose = true;
                break;
            case 'q':
                g_settings.queuesize = atoi(optarg);
                break;
            default:
                // Unexpected argument--abort parsing
                goto cleanup;
        }
    }

    g_settings.cwd = getcwd(NULL, 0);
    ret = 0;
cleanup:
    return ret;
}

// SIGINT handler that detects Ctrl-C and sets the "stop serving" flag
void sigint_handler(int signum) {
    blog(COLOR_RED "Ctrl-C (SIGINT) detected; shutting down..." COLOR_RESET);
    server_running = false;
}

int validFile(const char* filePath, FILE **response, struct stat *responseStats) {
    char fullPath[PATH_MAX];

    if(stat(filePath + (filePath[0] == '/'), responseStats) != 0) {
        return -1;
    }

    if(!S_ISREG(responseStats->st_mode)) {
        return -2;
    }

    realpath(filePath + (filePath[0] == '/'), fullPath);
    if(strstr(fullPath, g_settings.cwd) != fullPath) {
        return -2;
    }

    if((*response = fopen(filePath + (filePath[0] == '/'), "rb")) == NULL) {
        return -1;
    }

    return 0;
}

int parseHeader(FILE *stream, FILE **response, struct stat *responseStats, char **responseCode, char **contentType, char **error) {
    char headerBuff[PATH_MAX];
    char **header = NULL;
    int ret = -1;

    if(fgets(headerBuff, sizeof headerBuff - 1, stream) != NULL && headerBuff[strlen(headerBuff) - 1] == '\n') {
        header = strSplit(headerBuff);
        if(splitLen(header) != 3) {
            *responseCode = "HTTP/1.0 400 Bad Request";
            *error = ERR_FORMAT;
            goto cleanup;
        } else if(strcmp(header[0], "GET") != 0) {
            *responseCode = "HTTP/1.0 501 Not Implemented";
            *error = ERR_NOT_IMPLEMENTED;
            goto cleanup;
        } else if(strlen(header[2]) != 8 || strstr(header[2], "HTTP/") != header[2] || !isdigit(header[2][5]) || header[2][6] != '.' || !isdigit(header[2][7])) {
            *responseCode = "HTTP/1.0 400 Bad Request";
            *error = ERR_FORMAT;
            goto cleanup;
        }
    } else {
        *responseCode = "HTTP/1.0 400 Bad Request";
        *error = ERR_OVERFLOW;
        goto cleanup;
    }

    if(strcmp(header[1], "/") == 0) { header[1] = "index.html"; } // Default to index.html
    int isValid = validFile(header[1], response, responseStats);
    if(isValid == -1) {
        *responseCode = "HTTP/1.0 404 Not Found";
        *error = ERR_NOT_FOUND;
        goto cleanup;
    } else if(isValid == -2) {
        *responseCode = "HTTP/1.0 403 Forbidden";
        *error = ERR_FORBIDDEN;
        goto cleanup;
    }
    *contentType = getContentType(header[1]);
    ret = 0;

cleanup:
    free(header);
    return ret;
}

void *handle_client(void *arg) {
    struct client_info *client = arg;
    FILE *stream = NULL;
    FILE *response = NULL;
    struct stat responseStats;
    char *contentType = "text/plain";
    char *responseCode = "HTTP/1.0 200 OK";
    char *error = "ERROR";
    char buff[256];

    if(g_settings.verbose) blog(COLOR_YELLOW "Handling Connection: %s:%d" COLOR_RESET, client->ip, client->port);

    // Make the socket a read/write file stream for ease of use
    if ((stream = fdopen(dup(client->fd), "r+")) == NULL) {
        perror("unable to wrap socket");
        goto cleanup;
    }

    if(parseHeader(stream, &response, &responseStats, &responseCode, &contentType, &error) != 0) {
        goto error;
    }

    eatStream(&stream);

    fprintf(stream, "%s\r\n", responseCode);
    fprintf(stream, "Content-Type: %s\r\n", contentType);
    fprintf(stream, "Content-Length: %d\r\n\r\n", (int) responseStats.st_size);

    size_t numRead;
    while((numRead = fread(buff, 1, sizeof buff, response)) == sizeof buff) {
        fwrite(buff, 1, sizeof buff, stream);
    }
    fwrite(buff, 1, numRead, stream);   // Write any data that is not % buffer size

    goto cleanup;

error:
    eatStream(&stream);
    fprintf(stream, "%s\r\n", responseCode);
    fprintf(stream, "Content-Type: %s\r\n", contentType);
    fprintf(stream, "Content-Length: %d\r\n\r\n", (int) strlen(error));
    fprintf(stream, "%s", error);

cleanup:
    if (stream) fclose(stream);
    blog(COLOR_YELLOW "Client %s:%d disconnected." COLOR_RESET, client->ip, client->port);
    destroy_client_info(client);
    free(client);
    return NULL;
}

// This method actually runs the server. It listens for connections, then
// adds the connection to the worker thread pool for handling.
// <pool> The worker thread pool/queue
// <server_sock> The socket that the server is connected on.
void runServer(threadpool_t *pool, int *server_sock) {
    server_running = true;
    while (server_running) {
        client_info_t *client = malloc(sizeof(client_info_t));

        // Wait for a connection on that socket
        if (wait_for_client(*server_sock, client)) {
            if (errno != EINTR) { perror("unable to accept connection"); }
        } else {
            if(threadpoolAdd(pool, client) != 0) {
                blog(COLOR_RED "Kicking %s:%d: Max Connections Exceeded." COLOR_RESET, client->ip, client->port);
                destroy_client_info(client);    // Max Connections Exceeded
            } else {
                blog(COLOR_GREEN "Connection from %s:%d. [%d] total threads." COLOR_RESET, client->ip, client->port, pool->active);
            }
        }
    }
}

int main(int argc, char **argv) {
    int ret = 1;

    // Network server/client context
    int server_sock = -1;

    threadpool_t *pool = NULL;

    // Handle our options
    if (parse_options(argc, argv)) {
        printf("Usage: %s [-p PORT] [-h HOSTNAME/IP] [-r HOMEDIRECTORY] [-w MAXWORKERS] [-v]\n", argv[0]);
        goto cleanup;
    }

    // Install signal handler for SIGINT
    struct sigaction sa_int = {
        .sa_handler = sigint_handler
    };
    if (sigaction(SIGINT, &sa_int, NULL) || signal(SIGPIPE, SIG_IGN) || signal(SIGABRT, SIG_IGN) || signal(SIGHUP, SIG_IGN)) {
        LOG_ERROR("sigaction(SIGINT, ...) -> '%s'", strerror(errno));
        goto cleanup;
    }

    // Start listening on a given port number
    server_sock = create_tcp_server(g_settings.bindhost, g_settings.bindport);
    if (server_sock < 0) {
        perror("unable to create socket");
        goto cleanup;
    }
    blog(COLOR_GREEN "Bound and listening on %s:%s" COLOR_RESET, g_settings.bindhost, g_settings.bindport);

    if((pool = threadpoolInit(g_settings.threadcount, g_settings.queuesize)) == NULL) {
        LOG_ERROR("thradpoolInit() -> '%s'", strerror(errno));
        goto cleanup;
    }
    pool->verbose = g_settings.verbose;

    printf("Bound And Listening...\n");

    runServer(pool, &server_sock);
    threadpoolDestroy(pool);    // Kill remaining connections, close threadpool
    ret = 0;

cleanup:
    free(g_settings.cwd);
    if (server_sock >= 0) close(server_sock);
    return ret;
}
