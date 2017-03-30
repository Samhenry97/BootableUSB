////////////////////////////////////////////////
// File:        utils.h                       //
// Author:      Sam Henry                     //
// Date:        March 17, 2017                //
// Class:       CpS 320, Unix Programming     //
// Desc:        Definitions of utility        //
//              methodsfor the webserver      //
////////////////////////////////////////////////

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>     // Other standard library functions
#include <string.h>     // Standard string functions
#include <stdbool.h>
#include <stdarg.h>     // Variadic argument lists (for blog function)
#include <stdio.h>
#include <unistd.h>     // Standard system calls
#include <time.h>       // Time/date formatting function (for blog function)
#include <sys/stat.h>   // File stats
#include <limits.h>		// For PATH_MAX

// These macros are for coloring the output inside of the
// terminal. The format for printing with color is
// COLOR_[color] "message" COLOR_RESET
// http://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

// This struct is used for settings throughout the entire program.
struct settings {
    const char *bindhost;   // Hostname/IP address to bind/listen on
    const char *bindport;   // Portnumber (as a string) to bind/listen on
    char *cwd;				// Current working directory
    int threadcount;        // Max worker threads
    int queuesize;			// Thread Pool's queue size
    bool verbose;           // Extra debug information?
};

// This function is a handy wrapper to write a message
// to the console with a timestamp. The format of output is
// "timestamp:pid:user-defined-message"
// <fmt> The message
// <...> The extra parameters (printf style)
void blog(const char *fmt, ...);

// This function splits a string on a space, dynamically
// allocating in the process. That way, the string can have
// any number of spaces and still work
// <input> The input string to parse
// <return> The split string
char **strSplit(char *input);

// This function gets the content type of the file that is
// sent in. It looks for the last period in the file and
// uses the extension to give back the content type.
// <input> The file name to look at
// <return> The content type of the file
char *getContentType(char *input);

// Determines the length of a split string. it loops
// through the data, looking for a NULL terminator. 
// <data> The data to get the size of
// <return> The length of the split data
size_t splitLen(char **data);

// Given a file path, determines if the file is valid. If the file does not
// exist, returns a -1 for "file not found". If the file is not a regular
// file or does not lie in the current working directory, returns -2 for "forbidden".
// <filePath> The file to open
// <response> The file stream to assign the open file to
// <responseStats> The stats for the file
// <return> 0 on success, -1 on not found, -2 on forbidden.
int validFile(const char* filePath, FILE **response, struct stat *responseStats);

// Parses the header of an HTTP/1.0 request. It will return an error code and set
// the correct responses if any errors occur. 
// <stream> The input socket stream
// <response> The file to send back
// <responseStats> The stats for the file to send back
// <responseCode> The code of the error (or success)
// <contentType> The content type of the file
// <error> Any error messages
int parseHeader(FILE *stream, FILE **response, struct stat *responseStats, char **responseCode, char **contentType, char **error);

// This function reads a input socket stream until all the necessary data has been cleared.
// <stream> The stream (socket) to clear.
void eatStream(FILE **stream);

// This function handles a client connection. The HTTP request must
// always be GET, and the function will return the right HTTP response
// based on if the request is valid or not.
// <arg> The client info for this connection
void *handle_client(void *arg);

#endif // UTILS_H