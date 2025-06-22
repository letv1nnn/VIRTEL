#ifndef DEBUG_H
#define DEBUG_H

#include "chunk.h"
#include "vm.h"

void decodeChunk(Chunk *chunk);
void vmStack(VM *vm);
void testScanner(const char *source);

#endif
