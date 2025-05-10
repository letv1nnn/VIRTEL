#include "chunk.h"

void createChunk(Chunk* chunk) {
	createConstants(&chunk->constants);
	chunk->capacity = 8;
	chunk->count = 0;
	chunk->bytes = MALLOC_BUFFER(uint8_t, 8);
	if (chunk->bytes == NULL) {
		exit(1);
	}

	chunk->linesCapacity = 8;
	chunk->linesCount = 0;
	chunk->lines = MALLOC_BUFFER(LineStart, 8);

	if (chunk->bytes == NULL || chunk->lines == NULL) {
		exit(1);
	}
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
	if (chunk->count >= chunk->capacity) {
		uint8_t* new_bytes = REALLOC_BUFFER(uint8_t, chunk->bytes, chunk->capacity);
		if (new_bytes == NULL) {
			perror("Failed to grow chunk");
			exit(1);
		}
		chunk->bytes = new_bytes;
		chunk->capacity *= 2;
	}
	chunk->bytes[chunk->count++] = byte;
	if (chunk->linesCount > 0 && chunk->lines[chunk->linesCount - 1].line == line) {
		chunk->lines[chunk->linesCount - 1].count++;
	}
	else {
		if (chunk->linesCount >= chunk->linesCapacity) {
			LineStart* tmp = REALLOC_BUFFER(LineStart, chunk->lines, chunk->linesCapacity);
			if (tmp == NULL) {
				perror("Failed to grow lines buffer");
				exit(1);
			}
			chunk->linesCapacity *= 2;
			chunk->lines = tmp;
		}
		chunk->lines[chunk->linesCount++] = (LineStart){ line, 1 };
	}
}

int writeConstant(Chunk* chunk, double num, int line) {
	writeChunk(chunk, OP_CONSTANT, line);
	writeINConstant(&(chunk->constants), num);
	writeChunk(chunk, chunk->constants.count - 1, line);
	return chunk->constants.count;
}

void freeChunk(Chunk* chunk) {
	freeConstants(&(chunk->constants));
	free(chunk->bytes);
	free(chunk->lines);   
	chunk->bytes = NULL;
	chunk->lines = NULL;  
	chunk->count = 0;
	chunk->capacity = 1;
	chunk->linesCount = 0;   
	chunk->linesCapacity = 0;
}

int getLine(Chunk* chunk, size_t offset) {
	int current = 0;
	for (int i = 0; i < chunk->linesCount; i++) {
		current += chunk->lines[i].count;
		if (current > offset) {
			return chunk->lines[i].line;
		}
	}
	return -1;
}
