#ifndef chunk_h
#define chunk_h

#include "common.h"
#include "memory.h"
#include "constant.h"


typedef enum {
    OP_RETURN,
    OP_CONSTANT,

    // Arithmetic operators ccode
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_NEG,
} OpCode;

typedef struct {
    size_t capacity; // storage capacity
    size_t currentSize; // its current size
    uint8_t *instructions; // storage for bytecode
    ConstantBuffer buf; // buffer contain a const struct | for store numbers
    int *lines;
} Chunk;


void createChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
size_t writeConstant(Chunk *chunk, Value value);
void freeChunk(Chunk *chunk);

#endif
