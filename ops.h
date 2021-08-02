#if !defined(OPS_H)
#define OPS_H

#include <stdbool.h>
#include <stddef.h>

#include "debug.h"

// the status codes that can be returned by various operation callbacks in
// addition to the functions in evaluate.h
typedef enum op_status {
    OP_SUCCESS,
    OP_ERROR,  // generic error - mainly returned when the underlaying stack
               // implementation fails
    OP_SYNTAX_ERROR,  // returned when the expression has unrecognized tokens,
                      // missing operators or operands, or has excess tokens
    OP_DIV_BY_ZERO,
    OP_OVERFLOW,
    OP_UNDERFLOW,
    OP_MISMATCHED_PARENTHESES,
    OP_INVALID_OP,  // mainly returned when the FE_INVALID floating-point
                    // exception is raised
    OP_EMPTY,       // empty expression
} op_status;

typedef op_status (*op_cb)(long double *, ...);

struct operator{
    char *op;
    op_cb cb;
    unsigned char operands_num;
    unsigned char precedence;  // lower values = higher precedence
    unsigned pos;              // start index in expression string
};

struct operand {
    long double value;
    unsigned pos;  // start index in expression string
};

int get_operator(struct operator* op_struct, const char *expr);

#endif /* OPS_H */
