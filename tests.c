#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "evaluate.h"

eval_state state;

void eval_assert(const char *expr,
                 op_status expected_status,
                 long double expected_value) {
    op_status status = eval_with_state(&state, expr);
    assert(status == expected_status);
    if (status == OP_SUCCESS) {
        long double ret;
        eval_get_result(&state, &ret);
        assert(ret == expected_value);
    }
}

#define M_PI 3.14159265358979323846
#define M_E 2.71828182845904523536

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

    eval_assert("5 % 0", OP_DIV_BY_ZERO, 0);

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

    eval_assert("PI", OP_SUCCESS, M_PI);
    eval_assert("pi", OP_SUCCESS, M_PI);
    eval_assert("e", OP_SUCCESS, M_E);
    eval_assert("E", OP_SUCCESS, M_E);

    // TODO: test the rest of the operators

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

    eval_free(&state);

    return 0;
}
