#if !defined(DEBUG_H)
#define DEBUG_H

#ifdef EVAL_DEBUG
#include <stdio.h>

void log_msg(FILE *fd,
             const char *func,
             const char *file,
             int line,
             const char *fmt,
             ...);

#define LOG_MSG(msg) log_msg(stderr, __func__, __FILE__, __LINE__, msg)
#define LOG_FMSG(fmt, ...) \
    log_msg(stderr, __func__, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define FLOG_MSG(fd, msg) log_msg(fd, __func__, __FILE__, __LINE__, msg)
#define FLOG_FMSG(fd, fmt, ...) \
    log_msg(fd, __func__, __FILE__, __LINE__, fmt, __VA_ARGS__)

#define LOG_OPR_STACK(stack)                                    \
    do {                                                        \
        fputs("operand stack: ", stderr);                       \
        for (unsigned __i = 0; __i < (stack).length; __i++) {   \
            fprintf(stderr, "%Lg , ", (stack).data[__i].value); \
        }                                                       \
        fputc('\n', stderr);                                    \
    } while (0)

#define LOG_OP_STACK(stack)                                   \
    do {                                                      \
        fputs("operator stack: ", stderr);                    \
        for (unsigned __i = 0; __i < (stack).length; __i++) { \
            fprintf(stderr, "'%s' , ", (stack).data[__i].op); \
        }                                                     \
        fputc('\n', stderr);                                  \
    } while (0)

#else
#define LOG_MSG(msg)
#define LOG_FMSG(fmt, ...)
#define FLOG_MSG(fd, msg)
#define FLOG_FMSG(fd, fmt, ...)
#define LOG_OPR_STACK(stack)
#define LOG_OP_STACK(stack)
#endif

#endif /*  DEBUG_H */
