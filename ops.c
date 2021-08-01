#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ops.h"

static op_status add(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double operand_1 = va_arg(args, long double);
    long double operand_2 = va_arg(args, long double);
    va_end(args);
    /* FIXME: does not detect overflow properly? Floating point numbers are a
     * PITA! */
    if (operand_1 > LDBL_MAX - operand_2)
        return OP_OVERFLOW;
    *res = operand_1 + operand_2;
    return OP_SUCCESS;
}

static op_status subtract(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double operand_1 = va_arg(args, long double);
    long double operand_2 = va_arg(args, long double);
    va_end(args);
    if (operand_1 < -LDBL_MAX + operand_2)
        return OP_UNDERFLOW;
    *res = operand_1 - operand_2;
    return OP_SUCCESS;
}

static op_status multiply(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double operand_1 = va_arg(args, long double);
    long double operand_2 = va_arg(args, long double);
    va_end(args);
    /* FIXME: check for overflow. */
    *res = operand_1 * operand_2;
    return OP_SUCCESS;
}

static op_status divide(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double operand_1 = va_arg(args, long double);
    long double operand_2 = va_arg(args, long double);
    va_end(args);
    if (operand_2 == 0)
        return OP_DIV_BY_ZERO;
    /* FIXME: check for overflow. */
    *res = operand_1 / operand_2;
    return OP_SUCCESS;
}

static op_status power(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double operand_1 = va_arg(args, long double);
    long double operand_2 = va_arg(args, long double);
    va_end(args);
    /* FIXME: check for overflow. */
    *res = powl(operand_1, operand_2);
    return OP_SUCCESS;
}

static op_status square_root(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double operand_1 = va_arg(args, long double);
    va_end(args);
    *res = sqrtl(operand_1);
    return OP_SUCCESS;
}

static op_status modulo(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    long double y = va_arg(args, long double);
    va_end(args);
    if (!y)
        return OP_DIV_BY_ZERO;
    *res = fmodl(x, y);
    return OP_SUCCESS;
}

static op_status factorial(long double* res, ...) {
    // FIXME: reject non-integers
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = 1;
    for (long double i = 2; i <= x; i++) {
        *res *= i;
    }
    return OP_SUCCESS;
}

static op_status sine(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = sinl(x);
    return OP_SUCCESS;
}

static op_status cosine(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = cosl(x);
    return OP_SUCCESS;
}

static op_status tangent(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = tanl(x);
    return OP_SUCCESS;
}

static op_status hsin(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = sinhl(x);
    return OP_SUCCESS;
}

static op_status hcos(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = coshl(x);
    return OP_SUCCESS;
}

static op_status htan(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = tanhl(x);
    return OP_SUCCESS;
}

static op_status arcsin(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = asinl(x);
    return OP_SUCCESS;
}

static op_status arccos(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = acosl(x);
    return OP_SUCCESS;
}

static op_status arctan(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = atanl(x);
    return OP_SUCCESS;
}

static op_status arcsinh(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = asinhl(x);
    return OP_SUCCESS;
}

static op_status arccosh(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = acoshl(x);
    return OP_SUCCESS;
}

static op_status arctanh(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = atanhl(x);
    return OP_SUCCESS;
}

static op_status ln(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double x = va_arg(args, long double);
    va_end(args);
    *res = logl(x);
    return OP_SUCCESS;
}

static op_status log_base(long double* res, ...) {
    va_list args;
    va_start(args, res);
    long double base = va_arg(args, long double);
    long double x = va_arg(args, long double);
    va_end(args);
    if (base == 2) {
        *res = log2l(x);
    } else if (base == 10) {
        *res = log10l(x);
    } else
        *res = logl(x) / logl(base);
    return OP_SUCCESS;
}

static op_status pi_constant(long double* res, ...) {
    *res = 3.14159265358979323846;
    return OP_SUCCESS;
}

static op_status e_constant(long double* res, ...) {
    *res = 2.71828182845904523536;
    return OP_SUCCESS;
}

static op_status op_abort(long double* res, ...) {
    fprintf(stderr, "BUG: Calling a callback on parentheses. Aborting...");
    exit(EXIT_FAILURE);

    return OP_ERROR;
}

#define OPS_NUM 26
#define DEF_OP(_op, _cb, _operands_num, prec)                \
    (struct operator) {                                      \
        .op = _op, .cb = _cb, .operands_num = _operands_num, \
        .precedence = prec                                   \
    }

static const struct operator operators[OPS_NUM] = {

    DEF_OP("(", op_abort, 0, 0),      DEF_OP(")", op_abort, 0, 0),
    DEF_OP("*", multiply, 2, 2),      DEF_OP("/", divide, 2, 2),
    DEF_OP("+", add, 2, 3),           DEF_OP("-", subtract, 2, 3),
    DEF_OP("^", power, 2, 1),         DEF_OP("%", modulo, 2, 1),
    DEF_OP("!", factorial, 1, 1),     DEF_OP("sqrt", square_root, 1, 1),
    DEF_OP("sin", sine, 1, 1),        DEF_OP("cos", cosine, 1, 1),
    DEF_OP("tan", tangent, 1, 1),     DEF_OP("sinh", hsin, 1, 1),
    DEF_OP("cosh", hcos, 1, 1),       DEF_OP("tanh", htan, 1, 1),
    DEF_OP("arcsin", arcsin, 1, 1),   DEF_OP("arccos", arccos, 1, 1),
    DEF_OP("arctan", arctan, 1, 1),   DEF_OP("arcsinh", arcsinh, 1, 1),
    DEF_OP("arccosh", arccosh, 1, 1), DEF_OP("arctanh", arctanh, 1, 1),
    DEF_OP("ln", ln, 1, 1),           DEF_OP("log", log_base, 2, 1),
    DEF_OP("PI", pi_constant, 0, 0),  DEF_OP("e", e_constant, 0, 0),
};

/* find a registered operator contained in `expr` */
int get_operator(struct operator* op_struct, const char* expr) {
    const struct operator* cur_cand = NULL;
    unsigned char cur_cand_len = 0;
    for (unsigned char i = 0; i < OPS_NUM; i++) {
        unsigned char j;
        for (j = 0; operators[i].op[j] && expr[j]; j++) {
            if (operators[i].op[j] != expr[j])
                break;
        }
        if (!operators[i].op[j]) {
            if (j > cur_cand_len) {
                // found a longer match
                cur_cand = &operators[i];
                cur_cand_len = j;
            }
        }
    }
#ifdef EVAL_DEBUG
    if (cur_cand) {
        LOG_FMSG("found operator '%s'", cur_cand->op);
    } else {
        LOG_FMSG("found no operator in '%s'", expr);
    }
#endif

    if (cur_cand) {
        memcpy(op_struct, cur_cand, sizeof(struct operator));
        return 1;
    }
    return 0;
}
