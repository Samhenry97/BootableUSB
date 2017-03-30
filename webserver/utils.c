////////////////////////////////////////////////
// File:        utils.c                       //
// Author:      Sam Henry                     //
// Date:        March 17, 2017                //
// Class:       CpS 320, Unix Programming     //
// Desc:        Declarations of utility       //
//              methods for the webserver     //
////////////////////////////////////////////////

#include "utils.h"

void blog(const char *fmt, ...) {
    // Convert user format string and variadic args into a fixed string buffer
    char user_msg_buff[256];
    va_list vargs;
    va_start(vargs, fmt);
    vsnprintf(user_msg_buff, sizeof(user_msg_buff), fmt, vargs);
    va_end(vargs);

    // Get the current time as a string
    time_t t = time(NULL);
    struct tm *tp = localtime(&t);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tp);

    // Print said string to STDOUT prefixed by our timestamp and pid indicators
    printf("[Server]: %s:%d: %s\n"  , timestamp, getpid(), user_msg_buff);
}

char **strSplit(char *input) {
    char **ret = NULL;
    size_t total = 1;
    char *tmp = input;
    char *lastSpace = NULL;
    char *strtokSave = input;

    while(*tmp != '\n') {
        if(*tmp == ' ') {
            if(lastSpace == NULL || tmp != lastSpace + 1) { total++; }
            lastSpace = tmp;
        }
        tmp++;
    }

    total += lastSpace == NULL || lastSpace < (input + strlen(input) - 1);
    ret = malloc(sizeof(char*) * total);

    if(ret) {
        size_t index = 0;
        char *token = strtok_r(input, " \r\n", &strtokSave);

        while(token) {
            *(ret + index++) = token;
            token = strtok_r(NULL, " \r\n", &strtokSave);
        }
 
        *(ret + index) = NULL;
    }

    return ret;
}

char *getContentType(char *input) {
    char *tmp = strrchr(input, '.');
    tmp = tmp == NULL ? input + strlen(input) : tmp + 1;

    if(strcmp(tmp, "html") == 0 || strcmp(tmp, "htm") == 0) {
        return "text/html";
    } else if(strcmp(tmp, "gif") == 0) {
        return "image/gif";
    } else if(strcmp(tmp, "jpeg") == 0 || strcmp(tmp, "jpg") == 0) {
        return "image/jpeg";
    } else if(strcmp(tmp, "png") == 0) {
        return "image/png";
    } else if(strcmp(tmp, "css") == 0) {
        return "text/css";
    } else if(strcmp(tmp, "txt") == 0) {
        return "text/plain";
    } else {
        return "application/octet-stream";
    }
}

size_t splitLen(char **data) {
    size_t ret = 0;
    char **tmp = data;
    while(*tmp++) ret++;
    return ret;
}

void eatStream(FILE **stream) {
    char buff[PATH_MAX];
    while(fgets(buff, sizeof buff, *stream) != NULL && strcmp(buff, "\r\n") != 0 && strcmp(buff, "\n") != 0) { ; }
}