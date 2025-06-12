#include "constant.h"

// basically the same as I've done with the chunk

void createConstantBuffer(ConstantBuffer *buf) {
    buf->values = NULL;
    buf->capacity = 0;
    buf->currentSize = 0;
}

void writeConstantBuffer(ConstantBuffer *buf, Value val) {
    if (buf->capacity <= buf->currentSize) {
        size_t oldCapacity = buf->capacity;
        buf->capacity = GROW_CAPACITY(oldCapacity);
        buf->values = GROW_BUFFER(Value, buf->values, oldCapacity, buf->capacity);
    }

    buf->values[buf->currentSize] = val;
    buf->currentSize++;
}

void freeConstantBuffer(ConstantBuffer *buf) {
    FREE_BUFFER(Value, buf->values, buf->capacity);
    createConstantBuffer(buf);
}

