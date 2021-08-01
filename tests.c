#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "evaluate.h"

ops_stack ops_stk;
operands_stack operands_stk;

void eval_assert(const char* expr,
                 op_status expected_status,
                 long double expected_value) {
    op_status status = eval_stk(&ops_stk, &operands_stk, expr);
    assert(status == expected_status);
    if (status == OP_SUCCESS) {
        long double ret = STACK_POP(operands_stk)->value;
        assert(ret == expected_value);
    }
    ops_stk.length = 0;
    operands_stk.length = 0;
}

int main(void) {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    if (STACK_INIT(ops_stk))
        return EXIT_FAILURE;
    if (STACK_INIT(operands_stk))
        return EXIT_FAILURE;

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

    return 0;
}
