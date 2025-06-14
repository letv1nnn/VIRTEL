#ifndef COMMON_H
#define COMMON_H


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#if 0
inline void mem_failed(void *ptr) {
    if (ptr == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }
}
#endif

#endif
