#include "common.h"
#include "chunk.h"
#include "debug.h"



int main(int argc, char **argv) {
    Chunk *chunk = constructChunk();

    writeNumber(chunk, 42, 1);
    writeNumber(chunk, 3.141592, 1);
    writeByte(chunk, OP_RETURN, 2);

    decodeChunk(chunk);

    deconstructChunk(chunk);
    return 0;
}


