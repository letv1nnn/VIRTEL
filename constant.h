#ifndef constant_h
#define constant_h

#include "common.h"
#include "memory.h"

typedef double Value;

typedef struct {
    size_t capacity;
    size_t currentSize;
    Value *values;
} ConstantBuffer;


void createConstantBuffer(ConstantBuffer *buf);
void writeConstantBuffer(ConstantBuffer *buf, Value val);
void freeConstantBuffer(ConstantBuffer *buf);

#endif
