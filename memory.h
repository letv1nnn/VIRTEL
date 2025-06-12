#ifndef memory_h
#define memory_h


#include "common.h"

// memory allocation macros
#define GROW_CAPACITY(cap) ((cap) < 8 ? 8 : (cap) * 2)
#define GROW_BUFFER(type, ptr, oldSize, newSize) \
    ((type*)reallocate(ptr, sizeof(type) * (oldSize), \
            sizeof(type) * (newSize)))
#define FREE_BUFFER(type, ptr, oldSize) \
    GROW_BUFFER(type, ptr, oldSize, 0)


void *reallocate(void *ptr, size_t oldSize, size_t newSize);

#endif
