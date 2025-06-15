#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"


int main(int argc, char **argv) {
    Chunk *chunk = constructChunk();
    constructVM(chunk);

    writeNumber(chunk, 42, 1);
    writeNumber(chunk, 3.141592, 1);
    writeByte(chunk, OP_ADD, 1);
    writeNumber(chunk, 123, 2);
    writeNumber(chunk, 90.909, 2);
    writeByte(chunk, OP_SUB, 2);
    writeByte(chunk, OP_MUL, 2);
    writeByte(chunk, OP_NEG, 3);
    writeByte(chunk, OP_RETURN, 4);

    interpret(chunk);

    //decodeChunk(chunk);

    //deconstructChunk(chunk);
    deconstructVM();
    return 0;
}


