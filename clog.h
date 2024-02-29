#ifndef _CLOG_H
#define _CLOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// TODO: Find a way to remove CLOG_INIT
// TODO: Implement timestamps
// TODO: Implement logging file info

typedef enum {
    CLOG_NONE    = -1,
    CLOG_DEBUG   =  0,
    CLOG_TRACE   =  1,
    CLOG_INFO    =  2,
    CLOG_WARNING =  3,
    CLOG_ERROR   =  4,
    CLOG_FATAL   =  5,
} ClogLevel;


#define CLOG_INIT clog_output_fd = stdout

#define clog_mute_level(lvl) clog_muted_level = lvl
#define clog_set_output(output_fd) clog_output_fd = output_fd
#define clog_set_fmt(fmt) clog_fmt = fmt

#define clog(level, ...) for (size_t i = 0; i < strlen(clog_fmt); i++) {                       \
                         char c = clog_fmt[i];                                                 \
                         if (c == '%') {                                                       \
                            char c = clog_fmt[++i];                                            \
                            switch (c)                                                         \
                            {                                                                  \
                            case 'c':                                                          \
                                if (clog_output_fd == stdout || clog_output_fd == stderr) {    \
                                    fprintf(clog_output_fd, "%s", clog_get_level_color(level));\
                                }                                                              \
                                break;                                                         \
                            case 'l':                                                          \
                                fprintf(clog_output_fd, "%s", clog_get_level_string(level));   \
                                break;                                                         \
                            case 'r':                                                          \
                                if (clog_output_fd == stdout || clog_output_fd == stderr) {    \
                                    fprintf(clog_output_fd, "\e[0m");                          \
                                }                                                              \
                                break;                                                         \
                            case 'm':                                                          \
                                fprintf(clog_output_fd, __VA_ARGS__);                          \
                                break;                                                         \
                            case '%':                                                          \
                                fprintf(clog_output_fd, "%c", '%');                            \
                                break;                                                         \
                            default:                                                           \
                                fprintf(clog_output_fd, "%c", c);                              \
                                break;                                                         \
                            }                                                                  \
                        }                                                                      \
                        else {                                                                 \
                            fprintf(clog_output_fd, "%c", c);                                  \
                        }                                                                      \
                    }                                                                          \
                    if (clog_output_fd == stdout || clog_output_fd == stderr) {                \
                        fprintf(clog_output_fd, "\e[0m\n");                                      \
                    }                                                                          \
                    else {fprintf(clog_output_fd, "\n");}

extern ClogLevel clog_muted_level;
extern FILE *clog_output_fd;
extern char *clog_fmt;

FILE *clog_output_fd = 0;
ClogLevel clog_muted_level = CLOG_NONE;
char *clog_fmt = "%c[%l]%r: %m";

const char *clog_get_level_string(ClogLevel level);
const char *clog_get_level_color(ClogLevel level);

const char *clog_get_level_string(ClogLevel level) {
    switch (level) {
        case CLOG_DEBUG:   return "DEBUG";
        case CLOG_TRACE:   return "TRACE";
        case CLOG_INFO:    return "INFO";
        case CLOG_WARNING: return "WARNING";
        case CLOG_ERROR:   return "ERROR";
        case CLOG_FATAL:   return "FATAL";
        default: case CLOG_NONE: {
            fprintf(stderr, "[FATAL]: Invalid ClogLevel supplied");
            exit(1);
        }
    }
}

const char *clog_get_level_color(ClogLevel level) {
    switch (level) {
        case CLOG_DEBUG:   return "\e[32m";
        case CLOG_TRACE:   return "\e[90m";
        case CLOG_INFO:    return "\e[97m";
        case CLOG_WARNING: return "\e[33m";
        case CLOG_ERROR:   return "\e[31m";
        case CLOG_FATAL:   return "\e[1m\e[91m";
        default: case CLOG_NONE:{
            fprintf(stderr, "[FATAL]: Invalid ClogLevel supplied");
            exit(1);
        }
    }
}


#endif //_CLOG_H