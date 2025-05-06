#include "common.h"
#include "chunk.h"

int main(int argc, char **argv) {

    Chunk chunk = createChunk();
    addConstant(&chunk, 0);
    addConstant(&chunk, 4.56);

    writeChunk(&chunk, OP_RETURN);
    displayChunk(&chunk);

    freeChunk(&chunk);   
    return 0;
	
	return 0;
}