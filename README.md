# Math Expression Evaluator

This is just a math expression evaluator in C that I wrote for fun and learning.
If you want a production-quality library, take a look at [TinyExpr](https://github.com/codeplea/tinyexpr).

## Description

The [Shunting-yard algorithm](https://en.wikipedia.org/wiki/Shunting-yard_algorithm) is used for parsing
expressions written in [Infix notation](https://en.wikipedia.org/wiki/Infix_notation) (e.g. `5 + 3`).
My implementation parses operators and operands and store each type in a separate stack (`ops_stack` and `operands_stack`)
for ease of computation, and doesn't provide a function to
produce an expression in [Reverse Polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation) yet.

## How to use

To use as a library, copy the necessary sources (all but demo.c, debug.*, tests.c) to your project
and include `evaluate.h`.
See [evaluate.h](evaluate.h) for some docs and [demo.c](demo.c) for a usage example.

## Repo structure

- [demo.c](demo.c) a simple calculator built using the library.
- [evaluate.c](evaluate.c) uses the [Shunting-yard algorithm](https://en.wikipedia.org/wiki/Shunting-yard_algorithm)
to evaluate expressions written in [infix notation](https://en.wikipedia.org/wiki/Infix_notation).
- [ops.c](ops.c) defines the supported operations.
- [stack.h](stack.h) is a stack implementation to support the algorithm. It's based on the [vector.h](vector.h) file, which
  I originally wrote for use in my C utilities, so it's generic (i.e macro hell).
- [debug.c](debug.c) prints some debugging info when the `EVAL_DEBUG` macro is defined.
- [tests.c](tests.c): some tests

## Building

To build and run the demo, run:
```
make demo
```

To run tests:

```
make tests
```

## TODO
- More operations and constants
- Explore more algorithms for mathematical expression parsing
- Rust version
- Use an arbitrary-precision arithmetic library to save myself from headaches
- [Operator associativity](https://en.wikipedia.org/wiki/Operator_associativity). Currently, the exponentiation operator (^) is special-cased to be right-associative.
  If I'll ever add more right-associative operators, I should modify the `operator` struct to store associativity and remove the special case.
