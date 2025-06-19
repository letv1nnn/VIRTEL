#include "include/debug.h"


void vmStack(VM *vm) {
    printf("Stack (top at %p): ", (void*)vm->stack_top);
    for (double *slot = vm->stack; slot < vm->stack_top; slot++) {
        printf("[ %f ] ", *slot);
    }
    printf("\n");
}

void decodeChunk(Chunk *chunk) {
    for (size_t i = 0; i < chunk->curr;) {
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
            case OP_ADD: case OP_SUB:
            case OP_MUL: case OP_DIV:
            case OP_NEG:
                char *type;
                if (opcode == OP_ADD) type = "OP_ADD";
                else if (opcode == OP_SUB) type = "OP_SUB";
                else if (opcode == OP_MUL) type = "OP_MUL";
                else if (opcode == OP_NEG) type = "OP_NEG";
                else type = "OP_DIV";
                printf("%04d - %s - <line %d>\n", chunk->opcodes[i], type, getLine(chunk, i));
                i++;
                break;
            default:
                printf("Unknown opcode\n");
                i++;
                break;
        }
    }

}

