#if !defined(DEBUG_H)
#define DEBUG_H

#ifdef EVAL_DEBUG
#include <stdio.h>

void log_msg(FILE* fd,
             const char* func,
             const char* file,
             int line,
             const char* fmt,
             ...);

#define LOG_MSG(msg) log_msg(stderr, __func__, __FILE__, __LINE__, msg)
#define LOG_FMSG(fmt, ...) \
    log_msg(stderr, __func__, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define FLOG_MSG(fd, msg) log_msg(fd, __func__, __FILE__, __LINE__, msg)
#define FLOG_FMSG(fd, fmt, ...) \
    log_msg(fd, __func__, __FILE__, __LINE__, fmt, __VA_ARGS__)

#else
#define LOG_MSG(msg)
#define LOG_FMSG(fmt, ...)
#define FLOG_MSG(fd, msg)
#define FLOG_FMSG(fd, fmt, ...)
#endif

#endif /*  DEBUG_H */
