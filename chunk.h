#ifndef VIRTEL_CHUNK_H
#define VIRTEL_CHUNK_H

#include "common.h"
#include "values.h"
#include "buffer.h"

//operation code enum
typedef enum {
	OP_CONSTANT,
	OP_RETURN,
} OpCode;

//structure for "Run-length encoding for line information" strategy
typedef struct {
	int line;
	int count;
} LineStart;

typedef struct {
	//chunk buffer structure
	size_t capacity;
	size_t count;
	uint8_t* bytes;
	// constants data
	Constants constants;
	// line information
	LineStart* lines;
	int linesCapacity;
	int linesCount;
} Chunk;

void createChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);
void writeConstant(Chunk* chunk, double num, int line);
int getLine(Chunk* chunk, size_t offset);

#endif