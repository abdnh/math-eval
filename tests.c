#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "evaluate.h"

// seems to be good enough
#define MY_EPSILON 0.0000000000000001L
// #define MY_EPSILON LDBL_EPSILON

// adapted from
// https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
bool almost_equal(long double A, long double B) {
    long double diff = fabsl(A - B);
    A = fabsl(A);
    B = fabsl(B);
    long double largest = fmaxl(A, B);
    LOG_FMSG(
        "A = %Lg , B = %Lg , diff = %Lg, MY_EPSILON = %Lg , largest = %Lg , "
        "largest * MY_EPSILON = %Lg",
        A, B, diff, MY_EPSILON, largest, largest * MY_EPSILON);
    if (diff <= largest * MY_EPSILON)
        return true;
    return false;
}

eval_state state;

void eval_assert(const char *expr,
                 op_status expected_status,
                 long double expected_value) {
    op_status status = eval_with_state(&state, expr);
    LOG_FMSG(
        "expr = '%s' , expected_status = %d , expected_value = %Lg , status = "
        "%d",
        expr, expected_status, expected_value, status);
    assert(status == expected_status);
    if (status == OP_SUCCESS) {
        long double ret;
        assert(eval_get_result(&state, &ret) == OP_SUCCESS);

        LOG_FMSG("%Lg == %Lg = %s", ret, expected_value,
                 almost_equal(ret, expected_value) ? "true" : "false");
        assert(almost_equal(ret, expected_value));
        // assert(ret == expected_value);
    }
}

#define EVAL_PI 3.14159265358979323846
#define EVAL_E 2.71828182845904523536

int main(void) {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    eval_init(&state);

    eval_assert("5 + 2", OP_SUCCESS, 7);
    eval_assert("5+2", OP_SUCCESS, 7);

    eval_assert("5 - 2", OP_SUCCESS, 3);
    eval_assert("5-2", OP_SUCCESS, 3);

    eval_assert("5 * 2", OP_SUCCESS, 10);
    eval_assert("5*2", OP_SUCCESS, 10);

    eval_assert("5 / 2", OP_SUCCESS, 2.5);
    eval_assert("5/2", OP_SUCCESS, 2.5);
    eval_assert("5 / 0", OP_DIV_BY_ZERO, 0);

    eval_assert("5 % 2", OP_SUCCESS, 1);
    eval_assert("5 % 0", OP_INVALID_OP, 0);

    eval_assert("5^2", OP_SUCCESS, 25);
    eval_assert("5^0", OP_SUCCESS, 1);

    eval_assert("3!", OP_SUCCESS, 6);
    eval_assert("3 !", OP_SUCCESS, 6);
    eval_assert("0!", OP_SUCCESS, 1);

    eval_assert("sqrt(4)", OP_SUCCESS, 2);
    eval_assert("sqrt(0)", OP_SUCCESS, 0);
    eval_assert("sqrt( 9 )", OP_SUCCESS, 3);
    eval_assert("sqrt ( 9 )", OP_SUCCESS, 3);
    eval_assert("sqrt (9)", OP_SUCCESS, 3);
    eval_assert("sin(PI/2)", OP_SUCCESS, 1);
    eval_assert("cos(PI)", OP_SUCCESS, -1);
    eval_assert("tan(0)", OP_SUCCESS, 0);
    eval_assert("sinh(0)", OP_SUCCESS, 0);
    eval_assert("cosh(0)", OP_SUCCESS, 1);
    eval_assert("tanh(0)", OP_SUCCESS, 0);
    eval_assert("arcsin(0)", OP_SUCCESS, 0);
    eval_assert("arccos(-1)", OP_SUCCESS, EVAL_PI);
    eval_assert("arctan(tan(pi/4))", OP_SUCCESS, EVAL_PI / 4);
    eval_assert("arcsin(0)", OP_SUCCESS, 0);
    eval_assert("arccosh(1)", OP_SUCCESS, 0);
    eval_assert("arctanh(0)", OP_SUCCESS, 0);
    eval_assert("ln(e)", OP_SUCCESS, 1);
    eval_assert("ln ( 1 )", OP_SUCCESS, 0);
    // made possible by the leniency of the parser
    eval_assert("log10(10)", OP_SUCCESS, 1);
    eval_assert("log2(2)", OP_SUCCESS, 1);
    eval_assert("log4(2)", OP_SUCCESS, 0.5);

    eval_assert("PI", OP_SUCCESS, EVAL_PI);
    eval_assert("pi", OP_SUCCESS, EVAL_PI);
    eval_assert("e", OP_SUCCESS, EVAL_E);
    eval_assert("E", OP_SUCCESS, EVAL_E);

    eval_assert("-5", OP_SUCCESS, -5);
    eval_assert("- 5", OP_SUCCESS, -5);
    eval_assert("5 -", OP_SYNTAX_ERROR, 5);
    eval_assert("+5", OP_SUCCESS, 5);
    eval_assert("+ 5", OP_SUCCESS, 5);
    eval_assert("5 +", OP_SYNTAX_ERROR, 5);
    eval_assert("* 5", OP_SYNTAX_ERROR, 5);
    eval_assert("5 *", OP_SYNTAX_ERROR, 5);

    eval_assert("5 + 2 * 2", OP_SUCCESS, 9);
    eval_assert("5 + 2 / 2", OP_SUCCESS, 6);
    eval_assert("(5 + 2) / 2", OP_SUCCESS, 3.5);
    eval_assert("5 + 2) / 2", OP_MISMATCHED_PARENTHESES, 3.5);
    eval_assert("(5 + 2 / 2", OP_MISMATCHED_PARENTHESES, 3.5);
    eval_assert("((5 + 2) / 2)", OP_SUCCESS, 3.5);
    eval_assert("2 * ((5 + 2) / 2)", OP_SUCCESS, 7);
    eval_assert("( 5 + 2", OP_MISMATCHED_PARENTHESES, 7);
    eval_assert("( 5 +", OP_SYNTAX_ERROR, 5);
    eval_assert("tan(10", OP_MISMATCHED_PARENTHESES, 0);

    eval_assert("gibberish", OP_SYNTAX_ERROR, 0);
    eval_assert("5+2 foobar", OP_SYNTAX_ERROR, 0);
    eval_assert("", OP_EMPTY, 0);
    eval_assert("  ", OP_EMPTY, 0);
    eval_assert("4 4", OP_SYNTAX_ERROR, 0);
    eval_assert("4 4 + 3", OP_SYNTAX_ERROR, 7);

    char expr_buf[100];
    sprintf(expr_buf, "%Lg + %Lg", LDBL_MAX, LDBL_MAX);
    eval_assert(expr_buf, OP_OVERFLOW, 0);

    sprintf(expr_buf, "%Lg * %Lg", LDBL_MIN, LDBL_MIN);
    eval_assert(expr_buf, OP_UNDERFLOW, 0);

    eval_free(&state);

    return 0;
}
