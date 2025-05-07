#include "common.h"
#include "chunk.h"
#include "virtualMachine.h"


int main(int argc, char **argv) {
	VirtualMachine vm;
	initVM(&vm);

	Chunk chunk;
	createChunk(&chunk);

	writeConstant(&chunk, 1.2, 1);
	writeConstant(&chunk, 3.4, 1);
	writeChunk(&chunk, OP_ADD, 1);
	writeConstant(&chunk, 5.6, 1);
	writeChunk(&chunk, OP_DIV, 1);
	writeChunk(&chunk, OP_NEG, 2);
	writeChunk(&chunk, OP_RETURN, 3);
	// displayChunk(&chunk);

	interpret(&vm, &chunk);

	freeChunk(&chunk);
	freeVM(&vm);

	return 0;
}
