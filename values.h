#ifndef VIRTEL_VALUES_H
#define VIRTEL_VALUES_H

#include "common.h"

typedef double Value;

typedef struct {
	int capacity, currSize;
	Value* values;
} Values;

Values *createValuesBuffer();
void addValue(Values* values, Value value);
void growValuesBuffer(Values* values);
void freeValuesBuffer(Values* values);

#endif