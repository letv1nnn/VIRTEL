#ifndef vm_h
#define vm_h

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "constant.h"

#define STACK_MAX 256 // max stack is only 256, because
                      // the instruction for the constant(bytecode that points to
                      // a constants buffer) is 8 bits, so only 256 different indeces

// hand vm a chunk of code and it runns it

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
}InterpretResult;

typedef struct {
    Chunk *chunk;
    uint8_t *ip; // instruction pointer

    Value stack[STACK_MAX]; // stack for storing the constants, and execute
    Value *stackTop;        // some simple arithmetic operations.
} VM;

void createVM();
void freeVM();
InterpretResult interpret(const char *source);

#endif
