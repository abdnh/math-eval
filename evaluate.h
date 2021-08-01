#if !defined(EVAL_H)
#define EVAL_H

#include "ops.h"
#include "stack.h"

STACK_DEF(ops_stack, struct operator);
STACK_DEF(operands_stack, struct operand);

/* Evaluate `expr`, using the provided stacks as buffers.
   The result can be obtained from `operands_stk`:
   ```
   long double result = STACK_POP(operands_stk)->value;
   ```
   Use this function to evaluate a bunch of expressions while avoiding
   initializing and freeing the stacks for each expression needlessly. The
   stacks must initialized before calling the function and freed afterwards. See
   demo.c
*/
op_status eval_stk(ops_stack* ops_stk,
                   operands_stack* operands_stk,
                   const char* expr);

/* Evaluate `expr` and store the result in `res` */
op_status eval(const char* expr, long double* res);

#endif
