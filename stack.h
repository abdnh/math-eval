#if !defined(STK_VEC_H)
#define STK_VEC_H

#include "vector.h"

#define STACK(T) VEC(T)

#define STACK_DEF(name, T) VEC_DEF(name, T)

#define STACK_INIT_N(stack, cap) VEC_INIT_N((stack), (cap))

#define STACK_INIT(stack) VEC_INIT((stack))

#define STACK_PUSH(stack, item) VEC_PUSH((stack), (item))

#define STACK_POP(stack) VEC_POP((stack))

#define STACK_PEEK(stack) VEC_AT((stack), (stack).length - 1)

#define STACK_FREE(stack) VEC_FREE((stack))

#endif
