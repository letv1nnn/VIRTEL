#ifndef MALLOC_H
#define MALLOC_H

#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define DEBUG_MODE 0

void *virtel_malloc(size_t size);
void virtel_free(void *ptr);
void *virtel_calloc(size_t nmemb, size_t size);
void *virtel_realloc(void *ptr, size_t size);


#endif
