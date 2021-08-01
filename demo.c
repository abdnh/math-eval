#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "evaluate.h"

#define error(msg) fprintf(stderr, "Error: %s\n", msg);

#define BUFFER_SIZE 1000

int main(void) {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    ops_stack ops_stk;
    if (STACK_INIT(ops_stk))
        return EXIT_FAILURE;
    operands_stack operands_stk;
    if (STACK_INIT(operands_stk))
        return EXIT_FAILURE;

    puts("Enter expressions to evaluate:");

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        op_status eval_ret = eval_stk(&ops_stk, &operands_stk, buffer);
        switch (eval_ret) {
            case OP_ERROR:
                error("internal error occured");
                break;
            case OP_SYNTAX_ERROR:
                error("syntax Error");
                break;
            case OP_DIV_BY_ZERO:
                error("division by zero");
                break;
            case OP_OVERFLOW:
                error("Overflow");
                break;
            case OP_UNDERFLOW:
                error("Underflow");
                break;
            case OP_MISMATCHED_PARENTHESES:
                error("mismatched parentheses");
                break;
            default: {
                long double res = STACK_POP(operands_stk)->value;
                printf("%Lf\n", res);
                fflush(stdout);
                break;
            }
        }
    }
    if (ferror(stdin)) {
        fputs("Fatal: error occured while processing input", stderr);
    }

    STACK_FREE(ops_stk);
    STACK_FREE(operands_stk);

    return 0;
}
