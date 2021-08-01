.PHONY: all clean demo build-tests tests build build-demo demo
.SUFFIXES:

CFLAGS:=-Wall
OBJ_FILES:=evaluate.o ops.o debug.o

all: demo

demo: build-demo
	./demo

build-demo: demo.c build
	$(CC) $(CFLAGS) demo.c $(OBJ_FILES) -o demo

build: $(OBJ_FILES)

evaluate.o: evaluate.h evaluate.c ops.o stack.h vector.h
	$(CC) $(CFLAGS) -c evaluate.c

ops.o: ops.h ops.c
	$(CC) $(CFLAGS) -c ops.c

debug.o: debug.h debug.c
	$(CC) $(CFLAGS) -c debug.c

tests: build-tests
	./tests

build-tests: tests.c build
	$(CC) $(CFLAGS) tests.c $(OBJ_FILES) -o tests

clean:
	rm -f *.o tests demo
