#include <ctype.h>
#include <errno.h>
#include <fenv.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "evaluate.h"
#ifdef EVAL_DEBUG
#include "debug.h"
#endif

bool eval_init(eval_state *state) {
    return !STACK_INIT(state->ops_stack) && !STACK_INIT(state->operands_stack);
}

void eval_free(eval_state *state) {
    STACK_FREE(state->ops_stack);
    STACK_FREE(state->operands_stack);
}

op_status eval_get_result(eval_state *state, long double *result) {
    struct operand *opr = STACK_POP(state->operands_stack);
    if (!opr)
        return OP_ERROR;
    *result = opr->value;
    return OP_SUCCESS;
}

static op_status eval_op_from_stk(eval_state *state) {
    const struct operator* op_struct = STACK_POP(state->ops_stack);
    LOG_FMSG("op = '%s' , operands_num = '%u' , precedence = '%u'",
             op_struct->op, (unsigned)op_struct->operands_num,
             (unsigned)op_struct->precedence);

    long double res = 0.0L;
    if (op_struct->operands_num == 0) {
        LOG_FMSG("evaluating '%s'", op_struct->op);
        feclearexcept(FE_ALL_EXCEPT);
        op_status ret = op_struct->cb(&res);
        if (ret != OP_SUCCESS) {
            return ret;
        }
    }
    if (op_struct->operands_num == 1) {
        if (state->operands_stack.length == 0) {
            LOG_FMSG(
                "found no operands for operator '%s', which takes one operand",
                op_struct->op);
            return OP_SYNTAX_ERROR;
        }
        long double operand_1 = STACK_POP(state->operands_stack)->value;
        LOG_FMSG("evaluating '%s' '%Lg'", op_struct->op, operand_1);
        feclearexcept(FE_ALL_EXCEPT);
        op_status ret = op_struct->cb(&res, operand_1);
        if (ret != OP_SUCCESS)
            return ret;
    } else if (op_struct->operands_num == 2) {
        if (state->operands_stack.length == 0) {
            LOG_FMSG(
                "found no operands for operator '%s', which takes two operands",
                op_struct->op);
            return OP_SYNTAX_ERROR;
        }
        struct operand *op2 = STACK_PEEK(state->operands_stack);
        struct operand *op1;
        if (state->operands_stack.length < 2 &&
            !(op2 && op2->pos > op_struct->pos &&
              (!strcmp(op_struct->op, "+") || !strcmp(op_struct->op, "-")))

        ) {
            // reject operators not valid as unary
            return OP_SYNTAX_ERROR;
        }
        op2 = STACK_POP(state->operands_stack);
        op1 = STACK_POP(state->operands_stack);
        long double val_2 = op2 ? op2->value : 0;
        long double val_1 = op1 ? op1->value : 0;

        LOG_FMSG("evaluating '%Lg' '%s' '%Lg'", val_1, op_struct->op, val_2);
        feclearexcept(FE_ALL_EXCEPT);
        op_status ret = op_struct->cb(&res, val_1, val_2);
        if (ret != OP_SUCCESS)
            return ret;
    }
    LOG_FMSG("pushing result %Lg", res);
    struct operand opr = {.value = res, .pos = UINT_MAX};
    if (STACK_PUSH(state->operands_stack, opr))
        return OP_ERROR;

    return OP_SUCCESS;
}

op_status eval_with_state(eval_state *state, const char *expr) {
    // reset stacks
    state->operands_stack.length = 0;
    state->ops_stack.length = 0;

    struct operator op_struct;
    for (unsigned i = 0; expr[i]; i++) {
        LOG_FMSG("expr starting from %u : '%s'", i, expr + i);

        if (isdigit(expr[i])) {
            char *end;
            struct operand opr;
            errno = 0;
            opr.value = strtold(&expr[i], &end);
            if (errno == ERANGE)
                return OP_OVERFLOW;
            opr.pos = i;
            i += (end - &expr[i] - 1);
            LOG_FMSG("pushing operand %Lg", opr.value);
            if (STACK_PUSH(state->operands_stack, opr))
                return OP_ERROR;
        } else if (get_operator(&op_struct, &expr[i])) {
            if (!strcmp(op_struct.op, "(")) {
                LOG_FMSG("pushing left parenthesis: %s\n", op_struct.op);
                if (STACK_PUSH(state->ops_stack, op_struct)) {
                    return OP_ERROR;
                }
            } else if (!strcmp(op_struct.op, ")")) {
                struct operator* top_op = NULL;
                bool found_left_parens = false;
                while (state->ops_stack.length) {
                    top_op = STACK_PEEK(state->ops_stack);
                    if (!strcmp(top_op->op, "(")) {
                        found_left_parens = true;
                        break;
                    }
                    op_status ret = eval_op_from_stk(state);
                    if (ret != OP_SUCCESS)
                        return ret;
                }
                if (!found_left_parens) {
                    return OP_MISMATCHED_PARENTHESES;
                } else {
                    // discard left parenthesis
                    STACK_POP(state->ops_stack);
                }
            } else {
                struct operator* top_op;
                while ((top_op = STACK_PEEK(state->ops_stack)) &&
                       strcmp(top_op->op, "(") &&
                       (op_struct.precedence > top_op->precedence ||
                        (op_struct.precedence == top_op->precedence &&
                         !(!strcmp(op_struct.op, "^") &&
                           !strcmp(op_struct.op, top_op->op))))) {
                    op_status ret = eval_op_from_stk(state);
                    if (ret != OP_SUCCESS)
                        return ret;
                }

                op_struct.pos = i;
                LOG_FMSG("pushing operator: %s\n", op_struct.op);
                if (STACK_PUSH(state->ops_stack, op_struct))
                    return OP_ERROR;
            }
            i += strlen(op_struct.op) - 1;
        } else if (!isspace(expr[i])) {
            // not a valid token
            return OP_SYNTAX_ERROR;
        }
    }

    while (state->ops_stack.length) {
        struct operator* top_op = STACK_PEEK(state->ops_stack);
        if (!strcmp(top_op->op, ")") || !strcmp(top_op->op, "(")) {
            return OP_MISMATCHED_PARENTHESES;
        }
        op_status ret = eval_op_from_stk(state);
        if (ret != OP_SUCCESS)
            return ret;
    }

    LOG_FMSG(
        "finished evaluation - ops_stk->length = %zu , operands_stk->length = "
        "%zu",
        state->ops_stack.length, state->operands_stack.length);

    if (!state->operands_stack.length) {
        return OP_EMPTY;
    }
    if (state->operands_stack.length > 1) {
        // excess operands
        LOG_OPR_STACK(state->operands_stack);
        return OP_SYNTAX_ERROR;
    }

    return OP_SUCCESS;
}

op_status eval(const char *expr, long double *res) {
    eval_state state;
    if (!eval_init(&state))
        return OP_ERROR;
    op_status ret = eval_with_state(&state, expr);
    if (ret == OP_SUCCESS) {
        ret = eval_get_result(&state, res);
    }
    eval_free(&state);

    return ret;
}
