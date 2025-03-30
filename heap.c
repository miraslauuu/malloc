#include "heap.h"
#include "custom_unistd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tested_declarations.h"
#include "rdebug.h"

struct heap_manager_t heap_manager = {0};

int compute_hash(memory_block_t* block) {
    int val = 0;
    size_t length = sizeof(memory_block_t) - sizeof(int);
    char *ptr = (char*)block;
    for (size_t i = 0; i < length; i++) {
        val += ptr[i];
    }
    return val;
}



static void set_fences(memory_block_t* block) {
    uint8_t* block_start = (uint8_t*)(block+1);
    memset(block_start, FENCE_BYTE, FENCE_SIZE);
    uint8_t* user_data = block_start + FENCE_SIZE;
    uint8_t* fence_end = user_data + block->size;
    memset(fence_end, FENCE_BYTE, FENCE_SIZE);
}

static memory_block_t* block_from_ptr(void* mem) {
    uint8_t* ptr = (uint8_t*)mem - FENCE_SIZE - sizeof(memory_block_t);
    return (memory_block_t*)ptr;
}

static void* ptr_from_block(memory_block_t* block) {
    return (uint8_t*)(block+1) + FENCE_SIZE;
}

int heap_setup(void) {
    heap_manager.heap_start = custom_sbrk(0);
    if (heap_manager.heap_start == (void*)-1) return -1;
    heap_manager.heap_size = 0;
    heap_manager.first_block = NULL;
    return 0;
}

void heap_clean(void) {
    if (heap_manager.heap_start) {
        custom_sbrk(-(intptr_t)heap_manager.heap_size);
    }
    memset(&heap_manager, 0, sizeof(heap_manager));
}

void update_hash(memory_block_t* block) {
    block->CODE = compute_hash(block);
}

static memory_block_t* request_space(size_t size) {
    size_t total_size = sizeof(memory_block_t) + 2*FENCE_SIZE + size;
    void* old_brk = custom_sbrk(total_size);
    if (old_brk == (void*)-1) return NULL;

    memory_block_t* block;
    if (!heap_manager.first_block) {
        block = (memory_block_t*)heap_manager.heap_start;
        block->prev = NULL;
        update_hash(block);
        block->next = NULL;
        update_hash(block);

        block->magic = 0xCAFEBABE;
        update_hash(block);

        heap_manager.first_block = block;
    } else {
        memory_block_t* curr = heap_manager.first_block;
        while (curr->next) curr = curr->next;
        block = (memory_block_t*)((uint8_t*)heap_manager.heap_start + heap_manager.heap_size);
        curr->next = block;
        update_hash(curr);

        block->prev = curr;
        update_hash(block);

        block->next = NULL;
        update_hash(block);

        block->magic = 0xCAFEBABE;
        update_hash(block);

    }

    block->size = size;
    update_hash(block);

    block->free = 0;
    update_hash(block);

    heap_manager.heap_size += total_size;

    set_fences(block);
    return block;
}

static memory_block_t* find_fit(size_t size) {
    memory_block_t* curr = heap_manager.first_block;
    while (curr) {
        if (curr->free && curr->size >= size) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void* heap_malloc(size_t size) {
    if (size == 0 || heap_validate() != 0) return NULL;
    memory_block_t* fit = find_fit(size);
    if (!fit) {
        fit = request_space(size);
        if (!fit) return NULL;
    } else {
        fit->free = 0;
        update_hash(fit);

        fit->size = size;
        update_hash(fit);

        set_fences(fit);
    }
    return ptr_from_block(fit);
}

void* heap_calloc(size_t nmemb, size_t size) {
    if (nmemb == 0 || size == 0 || heap_validate() != 0) return NULL;
    size_t total = nmemb * size;
    void* ptr = heap_malloc(total);
    if (ptr) memset(ptr, 0, total);
    return ptr;
}

static void merge_with_next(memory_block_t* block) {
    memory_block_t* next = block->next;
    if (!next || !next->free) return;
    size_t total_size = sizeof(memory_block_t) + 2*FENCE_SIZE + next->size;
    block->size += total_size;
    update_hash(block);

    block->next = next->next;
    update_hash(block);

    if (next->next) {

        next->next->prev = block;
        update_hash(next->next);

    }

    set_fences(block);
}

static void merge_with_prev(memory_block_t* block) {
    memory_block_t* prev = block->prev;
    if (!prev || !prev->free) return;
    size_t total_size = sizeof(memory_block_t) + 2*FENCE_SIZE + block->size;
    prev->size += total_size;
    update_hash(prev);

    prev->next = block->next;
    update_hash(prev);

    if (block->next){
        block->next->prev = prev;
        update_hash(block->next);

    }
    set_fences(prev);
}

void heap_free(void* memblock) {
    if (heap_validate() != 0) return;
    if (!memblock) return;

    pointer_type_t pt = get_pointer_type(memblock);
    if (pt != pointer_valid) return;

    memory_block_t* block = block_from_ptr(memblock);
    if (block->free) return;
    block->free = 1;
    update_hash(block);


    if (block->next) {
        size_t true_mb_size = (size_t) block->next -
                              (size_t) ((size_t)block + FENCE_SIZE * 2 + sizeof(memory_block_t));
        if (true_mb_size > block->size) {
            block->size = true_mb_size;
            update_hash(block);

        }
    }

    merge_with_next(block);
    merge_with_prev(block);
}



int heap_validate(void) {
    if (!heap_manager.heap_start) return 2;

    memory_block_t* curr = heap_manager.first_block;
    uint8_t* start = (uint8_t*)heap_manager.heap_start;
    uint8_t* end = start + heap_manager.heap_size;

    while (curr) {
        uint8_t* cptr = (uint8_t*)curr;
        if (cptr < start || cptr + sizeof(memory_block_t) > end) return 3;

        if (curr->prev) {
            uint8_t* pptr = (uint8_t*)curr->prev;
            if (pptr < start || pptr+sizeof(memory_block_t)>end) return 3;
            if (curr->magic != 0xCAFEBABE) return 3;
            if (curr->CODE != compute_hash(curr)) return 3;
            if (curr->prev->next != curr) return 3;
        }

        if (curr->next) {
            uint8_t* nptr = (uint8_t*)curr->next;
            if (nptr < start || nptr+sizeof(memory_block_t)>end) return 3;
            if (curr->next->prev != curr) return 3;
        }



        if (!curr->free) {
            uint8_t* fence_before = (uint8_t*)(curr+1);
            uint8_t* user_data = fence_before + FENCE_SIZE;
            uint8_t* fence_after = user_data + curr->size;
            for (int i = 0; i < FENCE_SIZE; i++) {
                if (fence_before[i] != FENCE_VALUE) return 1;
                if (fence_after[i] != FENCE_VALUE) return 1;
            }
        }

        curr = curr->next;
    }
    return 0;
}

static pointer_type_t internal_pointer_type(const void* pointer) {
    if (!heap_manager.heap_start) return pointer_unallocated;

    uint8_t* ptr = (uint8_t*)pointer;
    uint8_t* start = (uint8_t*)heap_manager.heap_start;
    uint8_t* end = start + heap_manager.heap_size;
    if (ptr < start || ptr >= end) return pointer_unallocated;

    memory_block_t* curr = heap_manager.first_block;
    while (curr) {
        uint8_t* cstart = (uint8_t*)(curr+1);
        uint8_t* ustart = cstart + FENCE_SIZE;
        uint8_t* uend = ustart + curr->size;
        uint8_t* cfend = uend;

        if (ptr >= (uint8_t*)curr && ptr < (uint8_t*)curr+sizeof(memory_block_t)) {
            return pointer_control_block;
        }

        if (ptr >= cstart && ptr < cstart+FENCE_SIZE) {
            if (curr->free) return pointer_unallocated;
            return pointer_inside_fences;
        }
        if (ptr >= cfend && ptr < cfend+FENCE_SIZE) {
            if (curr->free) return pointer_unallocated;
            return pointer_inside_fences;
        }

        if (ptr >= ustart && ptr < uend) {
            if (!curr->free) {
                if (ptr == (void*)ustart) {
                    return pointer_valid;
                } else {
                    return pointer_inside_data_block;
                }
            } else {
                return pointer_unallocated;
            }
        }

        curr = curr->next;
    }

    return pointer_unallocated;
}

pointer_type_t get_pointer_type(const void* const pointer) {
    int hv = heap_validate();


    if (pointer == NULL) return pointer_null;
    if (!heap_manager.heap_start && hv != 0) {
        if (hv == 2) return pointer_unallocated;
    }

    if (hv == 1) return pointer_heap_corrupted;

    if (hv == 2) return pointer_unallocated;
    pointer_type_t pt = internal_pointer_type(pointer);

    if (hv == 3) {

        if (pt == pointer_valid || pt == pointer_inside_data_block || pt == pointer_control_block || pt == pointer_inside_fences) {
            return pointer_heap_corrupted;
        } else {
            return pointer_unallocated;
        }
    }

    return pt;
}

void* heap_realloc(void* memblock, size_t size) {
    int hv = heap_validate();
    if (hv != 0) return NULL;

    if (memblock == NULL) {
        if (size == 0) return NULL;
        return heap_malloc(size);
    }

    pointer_type_t pt = get_pointer_type(memblock);
    if (pt != pointer_valid) {
        if (size == 0) return NULL;
        return NULL;
    }

    memory_block_t* block = block_from_ptr(memblock);
    if (size == 0) {
        heap_free(memblock);
        return NULL;
    }

    if (size == block->size) {
        return memblock;
    } else if (size < block->size) {
        block->size = size;
        update_hash(block);

        set_fences(block);
        return memblock;
    } else {
        size_t needed = size - block->size;
        memory_block_t* next = block->next;
        if (next && next->free) {
            size_t next_total = sizeof(memory_block_t) + 2*FENCE_SIZE + next->size;
            if (next->size >= needed) {
                block->size += next_total;
                update_hash(block);

                block->next = next->next;
                update_hash(block);

                if (block->next) {
                    block->next->prev = block;
                    update_hash(block->next);

                }
                block->free = 0;
                update_hash(block);

                block->size = size;
                update_hash(block);

                set_fences(block);
                return memblock;
            } else if (((size_t)next->next - (size_t)block - sizeof(next)) >= needed) {
                next->size = ((size_t)next->next - (size_t)next + sizeof(next)) - FENCE_SIZE*2;

                block->size += next_total;
                update_hash(block);

                block->next = next->next;
                update_hash(block);

                if (block->next) {
                    block->next->prev = block;
                    update_hash(block->next);

                }
                block->free = 0;
                update_hash(block);

                block->size = size;
                update_hash(block);

                set_fences(block);
                return memblock;
            }
        } else if (!next) {
            void* dummy = custom_sbrk(needed + FENCE_SIZE);
            if (dummy ==(void*)-1) {
                return 0;
            }
            heap_manager.heap_size += (needed + FENCE_SIZE);
            block->size = size;
            update_hash(block);
            set_fences(block);
            return memblock;
        }

        void* new_ptr = heap_malloc(size);
        if (!new_ptr) return NULL;
        memcpy(new_ptr, memblock, block->size);
        heap_free(memblock);
        return new_ptr;
    }
}

size_t heap_get_largest_used_block_size(void) {
    if (!heap_manager.heap_start) return 0;

    int status = heap_validate();
    if (status != 0) return 0;

    size_t max = 0;
    memory_block_t* curr = heap_manager.first_block;

    while (curr) {
        if (!curr->free && curr->size > max) {
            max = curr->size;
        }
        curr = curr->next;
    }

    return max;
}

