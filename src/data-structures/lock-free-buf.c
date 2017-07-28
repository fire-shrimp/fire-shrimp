#include "lock-free-buf.h"
#include "error.h"
#include "mem.h"

void init_lock_free_buf(lock_free_buf *buf, uint32_t capacity, size_t stride)
{
    buf->values = mem_alloc(STATIC_MEM, capacity * stride, 0, "lock_free_buf");
    buf->capacity = capacity;
    buf->count = 0;
    buf->stride = stride;
}

// not thread safe. use only from main thread
void reset_lock_free_buf(lock_free_buf *buf)
{
    buf->count = 0;
}

// thread safe
void *push_lock_free_buf(lock_free_buf *buf)
{
    ASSERT(buf, "buf is null");
    ASSERT(buf->values, "buf values is null");

    atomic_int i = atomic_fetch_add_explicit(&buf->count, 1, memory_order_seq_cst);

    ASSERT(i < (buf->capacity - 1), "lock_free_buf overflow");

    return ((uint8_t *)buf->values + (buf->stride * i));
}

// not thread safe. use only from main thread
void *get_at_index_lock_free_buf(lock_free_buf *buf, uint32_t index)
{
    ASSERT(buf, "buf is null");
    ASSERT(index < buf->capacity, "invalid index");

    return ((uint8_t *)buf->values + (buf->stride * index));
}
