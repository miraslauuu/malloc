#ifndef MY_MALLOC_HEAP_H
#define MY_MALLOC_HEAP_H
#include <stddef.h>
#include <stdint.h>

#define FENCE_BYTE 0xDE
#define FENCE_SIZE 16
#define FENCE_VALUE 0xDE

typedef enum pointer_type_t {
    pointer_null = 0,
    pointer_heap_corrupted = 1,
    pointer_control_block = 2,
    pointer_inside_fences = 3,
    pointer_inside_data_block = 4,
    pointer_unallocated = 5,
    pointer_valid = 6
} pointer_type_t;
int heap_setup(void);
void heap_clean(void);

void* heap_malloc(size_t size);
void* heap_calloc(size_t nmemb, size_t size);
void* heap_realloc(void* memblock, size_t size);
void  heap_free(void* memblock);

size_t heap_get_largest_used_block_size(void);
pointer_type_t get_pointer_type(const void* const pointer);
int heap_validate(void);

typedef struct __attribute__((__packed__)) memory_block_t {
    size_t size;
    int free;
    struct memory_block_t* next;
    struct memory_block_t* prev;
    uint32_t magic;
    int CODE;
} memory_block_t;


typedef struct heap_manager_t{
    void* heap_start;
    size_t heap_size;
    memory_block_t* first_block;
} heap_manager_t;

extern struct heap_manager heap_mgmt;

#endif // MY_MALLOC_HEAP_H
