#include "common.h"
#include "chunk.h"


int main(int argc, char **argv) {
	Chunk chunk;
	createChunk(&chunk);

	writeConstant(&chunk, 123.456, 90);
	writeConstant(&chunk, 99.88, 1);
	writeChunk(&chunk, OP_RETURN, 12);
	displayChunk(&chunk);

	freeChunk(&chunk);

	return 0;
}