#ifndef VIRTEL_VALUES_H
#define VIRTEL_VALUES_H

#include "common.h"
#include "buffer.h"

typedef double Number;

typedef struct {
	size_t count;
	size_t capacity;
	Number* numbers;
} Constants;

void createConstants(Constants* constants);
void writeINConstant(Constants* constants, Number num);
void freeConstants(Constants* constants);

#endif