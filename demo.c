#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "evaluate.h"

#define error(msg) fprintf(stderr, "Error: %s\n", msg)

#define fatal(msg) fprintf(stderr, "Fatal: %s\n", msg)

#define BUFFER_SIZE 1000

int main(void) {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    eval_state state;
    if (!eval_init(&state)) {
        fatal("out of memory");
    }

    puts("Enter expressions to evaluate:");

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        op_status eval_ret = eval_with_state(&state, buffer);
        switch (eval_ret) {
            case OP_ERROR:
                error("internal error occured");
                break;
            case OP_SYNTAX_ERROR:
                error("invalid expression");
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
                long double res;
                if (eval_get_result(&state, &res) != OP_SUCCESS) {
                    fatal("unknown error occured");
                }
                printf("%Lf\n", res);
                fflush(stdout);
                break;
            }
        }
    }
    if (ferror(stdin)) {
        fatal("error occured while processing input");
    }

    eval_free(&state);

    return 0;
}
