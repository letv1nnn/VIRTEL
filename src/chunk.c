#include "chunk.h"


Chunk *constructChunk() {
    Chunk *chunk = virtel_malloc(sizeof(Chunk));
    if (chunk == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }
    chunk->cap = INIT_CAP;
    chunk->curr = 0;
    chunk->opcodes = (uint8_t *)virtel_malloc(INIT_CAP);
    chunk->constants = constructConstants();
    chunk->lines = (int *)virtel_malloc(INIT_CAP * sizeof(int));
    chunk->runs = (int *)virtel_malloc(INIT_CAP * sizeof(int));
    chunk->line_count = 0;
    if (chunk->opcodes == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }
    return chunk;
}


void deconstructChunk(Chunk *chunk) {
    if (chunk == NULL) return;
    virtel_free(chunk->opcodes);
    deconstructConstants(chunk->constants);
    chunk->curr = 0;
    chunk->cap = INIT_CAP;
    virtel_free(chunk->lines);
    virtel_free(chunk->runs);
    virtel_free(chunk);
}


// int write Byte function I need to make my buffer growing
// when the current size approached the capacity (*2).
// gonna use my realloc for this purpose.
void writeByte(Chunk *chunk, uint8_t byte, int line) {
    // growing process...
    if (chunk->curr + 1 > chunk->cap) {
        chunk->cap *= 2;
        chunk->opcodes = (uint8_t *)virtel_realloc(chunk->opcodes, chunk->cap);
        if (chunk->opcodes == NULL) {
            printf("Memory allocation failed");
            exit(1);
        }
    }

    if (chunk->line_count > 0 && chunk->lines[chunk->line_count - 1] == line) {
        chunk->runs[chunk->line_count - 1]++;
    } else {
        chunk->lines[chunk->line_count] = line;
        chunk->runs[chunk->line_count] = 1;
        chunk->line_count++;
    }

    chunk->opcodes[chunk->curr++] = byte;
}

// here I just need to add a NUMBERopcode to my buffer and right
// after it add an index of it in the Numbers array.
void writeNumber(Chunk *chunk, double number, int line) {
    writeByte(chunk, OP_CONSTANT, line);
    uint8_t index = writeConstants(chunk->constants, number);
    writeByte(chunk, index, line);
}

Constants *constructConstants() {
    Constants *constants = (Constants *)virtel_malloc(sizeof(Constants));
    if (constants == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }
    constants->cap = INIT_CAP;
    constants->curr = 0;
    constants->nums = (double *)virtel_malloc(INIT_CAP * sizeof(double));
    if (constants->nums == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }
    return constants;
}

void deconstructConstants(Constants *constants) {
    constants->cap = INIT_CAP;
    constants->curr = 0;
    virtel_free(constants->nums);
    virtel_free(constants);
}

uint8_t writeConstants(Constants *constants, double num) {
    if (constants->curr + 1 > constants->cap) {
        constants->cap *= 2;
        constants->nums = (double *)virtel_realloc(constants->nums, constants->cap * sizeof(double));
        if (constants->nums == NULL) {
            printf("Memory allocation failed!");
            exit(1);
        }
    }
    constants->nums[constants->curr++] = num;
    return constants->curr - 1;
}


int getLine(Chunk *chunk, size_t opcode_index) {
    size_t current_pos = 0;
    for (size_t i = 0; i < chunk->line_count; i++) {
        current_pos += chunk->runs[i];
        if (opcode_index < current_pos)
            return chunk->lines[i];
    }
    return -1;
}


