#include "debug.h"


void decodeChunk(Chunk *chunk) {
    for (int i = 0; i < chunk->curr;) {
        uint8_t opcode = chunk->opcodes[i];
        switch (opcode) {
            case (OP_CONSTANT):
                uint8_t const_index = chunk->opcodes[i + 1];
                printf("%04d - OP_CONSTANT - <line %d> - %f\n", opcode, getLine(chunk, i), chunk->constants->nums[const_index]);
                i += 2;
                break;
            case (OP_RETURN):
                printf("%04d - OP_RETURN - <line %d>\n", chunk->opcodes[i], getLine(chunk, i));
                i++;
                break;
            default:
                printf("Unknown opcode\n");
                i++;
                break;
        }
    }

}

