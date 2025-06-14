#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, char **argv) {
    Chunk *chunk = constructChunk();

    writeNumber(chunk, 42);
    writeByte(chunk, OP_RETURN);

    decodeChunk(chunk);

    deconstructChunk(chunk);
    return 0;
}
