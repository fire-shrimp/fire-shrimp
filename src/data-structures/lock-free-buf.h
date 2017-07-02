#pragma once

#include <stdint.h>
#include <stdatomic.h>

/*Supports concurrent writes, but not concurrent reads*/

typedef struct lock_free_buf {
    void *values;
    uint32_t capacity;
    atomic_int count;
    size_t stride;
} lock_free_buf;

void init_lock_free_buf(lock_free_buf *buf, uint32_t capacity, size_t stride);

void reset_lock_free_buf(lock_free_buf *buf);

void *push_lock_free_buf(lock_free_buf *buf);

void *get_at_index_lock_free_buf(lock_free_buf *buf, uint32_t i);
