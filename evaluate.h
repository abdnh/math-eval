#if !defined(EVAL_H)
#define EVAL_H

#include "ops.h"
#include "stack.h"

typedef struct eval_state {
    STACK(struct operator) ops_stack;
    STACK(struct operand) operands_stack;
} eval_state;

/* Initialize state required for evaluation.
   Call before eval_with_state()
*/
bool eval_init(eval_state *state);

/* Free the evaluation state.
   Should be called after finishing your work with this state instance.
*/
void eval_free(eval_state *state);

/* Evaluate `expr`, using the provided state instance.
   The result can be obtained using the eval_get_result() function.
   Use this function to evaluate a bunch of expressions while avoiding
   initializing and freeing the stacks for each expression needlessly.
   eval_init() must be called before using this function.
*/
op_status eval_with_state(eval_state *state, const char *expr);

/* Get evaluation result calculated after the last call to eval_with_state() */
op_status eval_get_result(eval_state *state, long double *result);

/* Evaluate `expr` and store the result in `res`.
   Convenient for one-off evaluations.
*/
op_status eval(const char *expr, long double *res);

#endif
