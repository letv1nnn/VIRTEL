#include "debug.h"


void decodeChunk(Chunk *chunk) {
    for (int i = 0; i < chunk->curr; i++) {
        switch (chunk->opcodes[i]) {
            case (OP_CONSTANT):
                printf("%04d - OP_CONSTANT - %f\n", chunk->opcodes[i++], chunk->constants->nums[i]);
                break;
            case (OP_RETURN):
                printf("%04d - OP_RETURN\n", chunk->opcodes[i]);
                break;
            default:
                printf("Unknown opcode\n");
                break;
        }
    }

}

