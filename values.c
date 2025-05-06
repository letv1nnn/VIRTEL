#include "values.h"

Values* createValuesBuffer() {
    Values* values = (Values*)malloc(sizeof(Values));
    isAllocated(values);
    values->capacity = 8;
    values->currSize = 0;
    values->values = (Value*)malloc(values->capacity * sizeof(Value));
    isAllocated(values->values);
    return values;
}

void addValue(Values* values, Value value) {
    if (values->currSize >= values->capacity) {
        growValuesBuffer(values);
    }
    values->values[values->currSize++] = value;
}

void growValuesBuffer(Values* values) {
    int newCapacity = values->capacity * 2;
    Value* newValues = (Value*)realloc(values->values, newCapacity * sizeof(Value));
    isAllocated(newValues);
    values->values = newValues;
    values->capacity = newCapacity;
}

void freeValuesBuffer(Values* values) {
    free(values->values);
    free(values);
}