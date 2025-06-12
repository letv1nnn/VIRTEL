#include "chunk.h"


void createChunk(Chunk *chunk) {
    chunk->capacity = 0;
    chunk->currentSize = 0;
    chunk->instructions = NULL;
    chunk->lines = NULL;
    createConstantBuffer(&chunk->buf);
}


void writeChunk(Chunk *chunk, uint8_t byte, int line) {
    if (chunk->capacity <= chunk->currentSize) {
        size_t oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->instructions = GROW_BUFFER(uint8_t, chunk->instructions, oldCapacity, chunk->capacity);
        chunk->lines = GROW_BUFFER(int, chunk->lines, oldCapacity, chunk->capacity);

    }

    chunk->instructions[chunk->currentSize] =  byte;
    chunk->lines[chunk->currentSize] = line;
    chunk->currentSize++;
}

size_t writeConstant(Chunk *chunk, Value value) {
    writeConstantBuffer(&chunk->buf, value);
    return chunk->buf.currentSize - 1;
}

void freeChunk(Chunk *chunk) {
    freeConstantBuffer(&chunk->buf);
    FREE_BUFFER(uint8_t, chunk->instructions, chunk->capacity);
    FREE_BUFFER(int, chunk->lines,chunk->capacity);
    createChunk(chunk);
}
