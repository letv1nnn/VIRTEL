#ifndef VIRTEL_CHUNK_H
#define VIRTEL_CHUNK_H

#include "common.h"
#include "values.h"

#include <stdbool.h>

typedef enum {
	OP_CONSTANT,
	OP_RETURN,
}OpCode;

typedef struct {
	int capacity, currSize;
	uint8_t* bytes;
	Values* constants;
} Chunk;

Chunk createChunk();
void freeChunk(Chunk *chunk);
void growChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, OpCode byte);
void displayChunk(Chunk *chunk);
void addConstant(Chunk* chunk, Value constant);

#endif