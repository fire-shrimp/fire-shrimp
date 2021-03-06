#pragma once

typedef void *(*mem_alloc_fn)(void *instance, size_t sz, const char *tag);

typedef void (*mem_free_fn)(void *instance, void *ptr, const char *tag);

typedef struct allocator_interface {
    mem_alloc_fn alloc_fn;
    mem_free_fn free_fn;
} allocator_interface;
