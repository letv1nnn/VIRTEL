#include "buffer.h"

void *reallocate(void* ptr, size_t oldCapacity) {
	if (oldCapacity == 0) oldCapacity = 1;
	void* result = realloc(ptr, oldCapacity * 2);
	if (result == NULL) {
		perror("Memory allocation failed.");
		exit(1);
	}
	return result;
}