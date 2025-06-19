#ifndef CHUNK_H
#define CHUNK_H


#include "common.h"
#include "malloc.h"
#include <stdlib.h>

#define INIT_CAP 8

typedef enum {
    OP_RETURN,
    OP_CONSTANT,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_NEG,
}Opcode;

// Numbers buffer
// for storing constants
typedef struct {
    size_t cap;
    size_t curr;
    double *nums;
}Constants;


typedef struct {
    size_t cap;
    size_t curr;
    uint8_t *opcodes;
    Constants *constants;
    // RLE
    int *lines;
    int *runs;
    size_t line_count;
}Chunk;

Chunk *constructChunk();
void deconstructChunk(Chunk *chunk);

void writeByte(Chunk *chunk, uint8_t byte, int line);
void writeNumber(Chunk *chunk, double number, int line);
Constants *constructConstants();
void deconstructConstants(Constants *constans);
// need to return an index of just added constant
uint8_t writeConstants(Constants *constants, double num);

// RLE, line querying
int getLine(Chunk *chunk, size_t opcode_index);

#endif
