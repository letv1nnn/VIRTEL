#include "debug.h"

void displayChunk(Chunk* chunk) {
    for (int i = 0; i < chunk->count; ) {   
        uint8_t instruction = chunk->bytes[i];
        int line = getLine(chunk, i);
        printf("Line: %4d\tInstruction: %04d\t", line, i);   

        switch (instruction) {
            case OP_CONSTANT: {
                uint8_t constant_idx = chunk->bytes[++i];   
                printf("OP_CONSTANT %g\n", chunk->constants.numbers[constant_idx]);
                break;
            }
            case OP_RETURN:
                printf("OP_RETURN\n");
                break;
            default:
                printf("Unknown opcode %d\n", instruction);
                break;
        }
        i++;
    }
}