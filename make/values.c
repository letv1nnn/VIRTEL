#include "values.h"

void createConstants(Constants* constants) {
	constants->capacity = 8;
	constants->count = 0;
	constants->numbers = MALLOC_BUFFER(Number, 8);
	if (constants->numbers == NULL) exit(1);
}

void writeINConstant(Constants *constants, Number num) {
	if (constants->capacity <= constants->count) {
		Number* new_bytes = REALLOC_BUFFER(Number, constants->numbers, constants->capacity, constants->capacity * 2);
		if (new_bytes == NULL) {
			perror("Failed to grow chunk");
			exit(1);
		}
		constants->capacity *= 2;
		constants->numbers = new_bytes;
	}
	constants->numbers[constants->count++] = num;
}

void freeConstants(Constants *constants) {
	free(constants->numbers);
	constants->numbers = NULL;
	constants->capacity = 1;
	constants->count = 0;
}
