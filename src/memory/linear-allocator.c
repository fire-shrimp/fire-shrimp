#include <stdlib.h>
#include <string.h>

#include "linear-allocator.h"
#include "error.h"

static void *mem_alloc(void *instance, size_t sz, const char *tag)
{
    linear_allocator *allocator = instance;

    size_t current = atomic_fetch_add_explicit(&allocator->offset, sz, memory_order_seq_cst);

    ASSERT(allocator->offset < allocator->sz, "linear_allocator overflow");

    void *ptr = allocator->buf + current;
    memset(ptr, 0, sz);
    return ptr;
}

static void mem_free(void *instance, void *ptr, const char *tag)
{
    linear_allocator *allocator = instance;
    allocator->offset = 0;
}

void init_linear_allocator(linear_allocator *allocator, void *ptr, size_t sz)
{
    ASSERT(ptr, "ptr is null");

    memset(ptr, 0, sz);

    allocator->interface.alloc_fn = mem_alloc;
    allocator->interface.free_fn = mem_free;
    allocator->buf = ptr;
    allocator->sz = sz;
    allocator->offset = 0;
}

