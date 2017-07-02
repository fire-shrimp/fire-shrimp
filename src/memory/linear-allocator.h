#pragma once

#include <stdatomic.h>

#include "allocator.h"

typedef struct linear_allocator {
    allocator_interface interface;
    uint8_t *buf;
    size_t sz;
    atomic_size_t offset;
} linear_allocator;

void init_linear_allocator(linear_allocator *allocator, void *ptr, size_t sz);
