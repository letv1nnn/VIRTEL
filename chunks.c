#include "chunk.h"

Chunk createChunk() {
	Chunk chunk;
	chunk.capacity = 8;
	chunk.currSize = 0;
	chunk.bytes = (uint8_t*)malloc(chunk.capacity * sizeof(uint8_t));
	isAllocated(chunk.bytes);
	chunk.constants = createValuesBuffer();
	return chunk;
}

void writeChunk(Chunk *chunk, OpCode byte) {
	if (chunk->currSize >= chunk->capacity) {
		growChunk(chunk);
	}
	chunk->bytes[chunk->currSize++] = byte;
}

void addConstant(Chunk* chunk, Value constant) {
	writeChunk(chunk, OP_CONSTANT);
	writeChunk(chunk, chunk->constants->currSize);
	addValue(chunk->constants, constant);
}

void freeChunk(Chunk *chunk) {
	free(chunk->bytes);
	chunk->capacity = 0;
	chunk->currSize = 0;
	freeValuesBuffer(chunk->constants);
}

void growChunk(Chunk *chunk) {
	int newCapacity = chunk->capacity * 2;
	uint8_t* newBytes = (uint8_t*)realloc(chunk->bytes, newCapacity * sizeof(uint8_t));
	isAllocated(newBytes);
	chunk->bytes = newBytes;
	chunk->capacity = newCapacity;
}


// diagnosis purposes
void displayChunk(Chunk *chunk) {
	for (int i = 0; i < chunk->currSize; ++i) {
		switch (chunk->bytes[i]) {
			case OP_RETURN: printf("OPCODE RETURN <code %d>\n", chunk->bytes[i]); break;
			case OP_CONSTANT:
				printf("OPCODE CONSTANT <code %d> - ", chunk->bytes[i]);
				uint8_t const_idx = chunk->bytes[++i];
				printf("<value %f>\n", chunk->constants->values[const_idx]);
				break;
			default: printf("Unknown upcode\n"); break;
		}
	}
}
