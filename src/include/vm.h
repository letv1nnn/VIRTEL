#ifndef VM_H
#define VM_H

#include "chunk.h"
#include "common.h"
#include "malloc.h"
#include "compiler.h"

#define STACK_MAX 1024
#define DEBUG 1

typedef enum {
    OK,
    RUN_ERR,
    COM_ERR,
} InterpretResult;


typedef struct VM {
    Chunk *chunk;
    uint8_t *ip;
    double stack[STACK_MAX];
    double *stack_top;
} VM;

void constructVM(Chunk *chunk);
void deconstructVM();
InterpretResult interpret(const char *source);
InterpretResult run();

#endif
