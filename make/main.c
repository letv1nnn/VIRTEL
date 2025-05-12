#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, char **argv) {
    createVM();


    Chunk chunk;
    createChunk(&chunk);

    size_t constant = writeConstant(&chunk, 42.0);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);

    constant = writeConstant(&chunk, 123.456);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);

    writeChunk(&chunk, OP_NEG, 2);
    writeChunk(&chunk, OP_DIV, 2);


    writeChunk(&chunk, OP_RETURN, 3);

    interpret(&chunk);


    freeVM();
    freeChunk(&chunk);
    return 0;
}
