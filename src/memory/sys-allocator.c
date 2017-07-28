
#include <stdlib.h>
#include <string.h>

#include "sys-allocator.h"

static void *mem_alloc(void *instance, size_t sz, const char *tag)
{
    void *ptr = malloc(sz);
    memset(ptr, 0, sz);
    return ptr;
}

static void mem_free(void *instance, void *ptr, const char *tag)
{
    free(ptr);
}

void init_sys_allocator(sys_allocator *allocator)
{
    allocator->interface.alloc_fn = mem_alloc;
    allocator->interface.free_fn = mem_free;
}

