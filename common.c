#include "common.h"

void isAllocated(void* ptr) {
    if (ptr == NULL) {
        printf("Memory allocation failed.");
        exit(1);
    }
}