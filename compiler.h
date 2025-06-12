#ifndef compiler_h
#define compiler_h

#include "common.h"
#include "scanner.h"
#include "vm.h"

bool compile(const char *source, Chunk *chunk);

#endif
