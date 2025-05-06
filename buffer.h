#ifndef VIRTEL_BUFFER_H
#define VIRTEL_BUFFER_H

#include "common.h"

#define MALLOC_BUFFER(type, capacity) ((type *)malloc((capacity) * sizeof(type)))
#define REALLOC_BUFFER(type, ptr, oldCapacity) \
	(type *)reallocate(ptr, oldCapacity * sizeof(type))
	
void *reallocate(void* ptr, size_t oldCapacity);

#endif
