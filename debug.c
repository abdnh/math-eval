#ifdef EVAL_DEBUG

#include <stdarg.h>
#include <stdio.h>
#include <time.h>


#include "debug.h"

void log_msg(FILE* fd,
             const char* func,
             const char* file,
             int line,
             const char* fmt,
             ...) {
    time_t t = time(NULL);
    struct tm* lt = localtime(&t);
    static char time_buf[16];
    time_buf[strftime(time_buf, sizeof(time_buf), "%H:%M:%S", lt)] = '\0';
    fprintf(fd, "%s %s@%s:%d - ", time_buf, func, file, line);
    va_list args;
    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    fputc('\n', fd);
    va_end(args);
}

#endif
