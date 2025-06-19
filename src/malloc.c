#include "include/malloc.h"


// The easiest way to implement malloc.
// Unfortunately, we won't be able to implement free with this malloc,
// because we do not know what size block is associated with it.
#if 0
void *malloc(size_t size) {
    void *p = sbrk(0);
    void *request = sbrk(size);
    if (request == (void *) -1)
        return NULL;
    assert(p == request);
    return p;
}
#endif

// Basically, a linked list.
typedef struct block_meta{
    size_t size;
    struct block_meta *next;
    int free;
#if DEBUG_MODE
    int magic;
#endif
} block_meta;


#define META_SIZE sizeof(block_meta)

// head of the linked list.
void *global_base = NULL;

block_meta *find_free_block(block_meta **last, size_t size) {
    block_meta *current = global_base;
    while (current != NULL && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current; // node which size fits with the requested or NULL otherwise(end of linked list).
}

block_meta *request_space(block_meta *last, size_t size) {
    block_meta *block;
    block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    assert((void*)block == request);
    if (request == (void *) -1)
        return NULL;
    if (last != NULL)
        last->next = block;
    block->size = size;
    block->next = NULL;
    block->free = 0;
#if DEBUG_MODE
    block->magic = 0x12345678;
#endif
    return block;
}

void *virtel_malloc(size_t size) {
    block_meta *block;
    if (size <= 0) {
        return NULL;
    }
    if (!global_base) {
        block = request_space(NULL, size);
        if (!block) {
            return NULL;
        }
        global_base = block;
    } else {
        block_meta *last = global_base;
        block = find_free_block(&last, size);
        if (!block) { // if there was not a free chunk of memory in the middle of the heap.
            block = request_space(last, size);
            if (!block) {
                return NULL;
            }
        } else { // Found free block
            block->free = 0;
#if DEBUG_MODE
            block->magic = 0x77777777;
#endif
        }
    }
    return (block + 1);
}


block_meta *get_block_ptr(void *ptr) {
    return (block_meta *)ptr - 1;
}


void virtel_free(void *ptr) {
    if (!ptr)
        return;
    block_meta *block_ptr = get_block_ptr(ptr);
    assert(block_ptr->free == 0);
#if DEBUG_MODE
    assert(block_ptr->magic == 0x12345678 || block_ptr->magic == 0x77777777);
    block_ptr->magic = 0x55555555;
#endif
    block_ptr->free = 1;
}


void *virtel_calloc(size_t q_elements, size_t element_size) {
    size_t size = q_elements * element_size;
    void *ptr = virtel_malloc(size);
    memset(ptr, 0, size);
    return ptr;
}


void *virtel_realloc(void *ptr, size_t newSize) {
    if (!ptr)
        return virtel_malloc(newSize);
    block_meta *block_ptr = get_block_ptr(ptr);
    if (block_ptr->size >= newSize) {
        return ptr;
    }
    void *new_ptr = virtel_malloc(newSize);
    if (!new_ptr)
        return NULL;
    memcpy(new_ptr, ptr, block_ptr->size);
    virtel_free(ptr);
    return new_ptr;
}
