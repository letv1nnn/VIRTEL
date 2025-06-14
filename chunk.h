#ifndef CHUNK_H
#define CHUNK_H


#include "common.h"
#include "malloc.h"
#include <stdlib.h>

#define INIT_CAP 8

typedef enum {
    OP_RETURN,
    OP_CONSTANT,

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
}Chunk;

Chunk *constructChunk();
void deconstructChunk(Chunk *chunk);

void writeByte(Chunk *chunk, uint8_t byte);
void writeNumber(Chunk *chunk, double number);

Constants *constructConstants();
void deconstructConstants(Constants *constans);
// need to return an index of just added constant
uint8_t writeConstants(Constants *constants, double num);

#endif
