#if !defined(OPS_H)
#define OPS_H

#include <stdbool.h>
#include <stddef.h>

#include "debug.h"

typedef enum op_status {
    OP_SUCCESS,
    OP_ERROR,
    OP_SYNTAX_ERROR,
    OP_DIV_BY_ZERO,
    OP_OVERFLOW,
    OP_UNDERFLOW,
    OP_MISMATCHED_PARENTHESES,
} op_status;

typedef op_status (*op_cb)(long double*, ...);

struct operator{
    char* op;
    op_cb cb;
    unsigned char operands_num;
    unsigned char precedence;  // lower values = higher precedence
    unsigned pos;              // start index in expression string
};

struct operand {
    long double value;
    unsigned pos;  // start index in expression string
};

int get_operator(struct operator* op_struct, const char* expr);

#endif /* OPS_H */
