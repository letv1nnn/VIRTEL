#include "debug.h"


void testChunk(Chunk *chunk) {
    printf("=== Chunk Debug ===\n");
    printf("Capacity: %zu, Size: %zu\n", chunk->capacity, chunk->currentSize);
    printf("Constants: %zu/%zu\n", chunk->buf.currentSize, chunk->buf.capacity);

    int i;
    for (i = 0; i < chunk->currentSize;) {
        uint8_t byte = chunk->instructions[i];
        int line = chunk->lines[i];
        printf("offset <%04d>  line <%d>  |  ", i, line);
        switch (byte) {
            case OP_CONSTANT:
                size_t index = chunk->instructions[i + 1];
                Value value = chunk->buf.values[index];
                printf("OP_CONSTANT  |  <value %lf>\n", value);
                i += 2;
                break;
            case OP_ADD:
                printf("OP_ADD\n");
                i++;
                break;
            case OP_SUB:
                printf("OP_SUB\n");
                i++;
                break;
            case OP_MUL:
                printf("OP_MUL\n");
                i++;
                break;
            case OP_DIV:
                printf("OP_DIV\n");
                i++;
                break;
            case OP_NEG:
                printf("OP_NEGATE\n");
                i++;
                break;
            case OP_RETURN:
                printf("OP_RETURN\n");
                i++;
                break;
            default:
                printf("Uknown upcode.\n");
                i++;
                break;
        }
    }
}

