# Math Expression Evaluator

This is just a math expression evaluator I wrote for fun and learning. Do not take seriously.

It's written in C, and I wish to add a Rust version in the future to practice my Rust skills.

## How to use

To use as a library, copy the necessary sources (all but demo.c, debug.*, tests.c) to your project
and include `evaluate.h`.
This header file exports two functions (`eval_stk` and `eval`) and two symbols (`ops_stack` and `operands_stack`).
See `evaluate.h` for some docs and `demo.c` for a usage example.

## Repo structure

- [demo.c](demo.c) a demo program that takes user input, try to evaluate it as an expression, and prints the result.
- [evaluate.c](evaluate.c) uses the [Shunting-yard algorithm](https://en.wikipedia.org/wiki/Shunting-yard_algorithm)
to evaluate expressions written in [infix notation](https://en.wikipedia.org/wiki/Infix_notation).
- [ops.c](ops.c) defines the supported operations.
- [stack.h](stack.h) is a stack implementation to support the algorithm. It's based on the [vector.h](vector.h) file, which
  I originally wrote for use in my C utilities, so it's generic (i.e macro hell).
- [debug.c](debug.c) prints some debugging info when the `EVAL_DEBUG` macro is defined.

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
- Rust version
- Explore more algorithms for mathematical expression parsing
