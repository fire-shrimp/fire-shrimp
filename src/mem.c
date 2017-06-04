
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "log.h"
#include "macros.h"
#include "mem.h"

struct mem_buf {
    uint8_t *buf;
    atomic_size_t offset;
    size_t size;
};

struct mem_buf glb_static_mem;
struct mem_buf glb_heap_mem;
struct mem_buf glb_frame_mem;
uint8_t *glb_memory;

void mem_init(size_t static_sz, size_t heap_sz, size_t frame_sz)
{
    size_t total = static_sz + heap_sz;

    void *ptr = malloc(total);

    ASSERT(ptr);

    memset(ptr, 0, total);

    glb_memory = ptr;

    glb_static_mem.buf = ptr;
    glb_static_mem.offset = 0;
    glb_static_mem.size = static_sz;

    glb_heap_mem.buf = glb_static_mem.buf + static_sz;
    glb_heap_mem.offset = 0;
    glb_heap_mem.size = heap_sz;

    glb_frame_mem.buf = glb_heap_mem.buf + heap_sz;
    glb_frame_mem.offset = 0;
    glb_frame_mem.size = frame_sz;
}

void *mem_alloc_static(size_t sz, size_t align, char *tag, char *file, uint32_t line)
{
    char *format = "[alloc:static][tag:%s][size:%d][align:%d][file:%s][line:%d]";
    LOG_DEBUG(MEMORY_LOG, format, tag, sz, align, file, line);

    size_t current = atomic_fetch_add_explicit(&glb_static_mem.offset, sz, memory_order_relaxed);

    LOG_ASSERT(glb_static_mem.offset < glb_static_mem.size, "out of static memory");

    void *ptr = glb_static_mem.buf + current;
    memset(ptr, 0, sz);
    return ptr;
}

void *mem_alloc_heap(size_t sz, size_t align, char *tag, char *file, uint32_t line)
{
    char *format = "[alloc:heap][tag:%s][size:%d][align:%d][file:%s][line:%d]";
    LOG_DEBUG(MEMORY_LOG, format, tag, sz, align, file, line);

    size_t current = atomic_fetch_add_explicit(&glb_heap_mem.offset, sz, memory_order_relaxed);

    LOG_ASSERT(glb_heap_mem.offset < glb_heap_mem.size, "out of heap memory");

    void *ptr = glb_heap_mem.buf + current;
    memset(ptr, 0, sz);
    return ptr;
}

void mem_free_heap()
{
}

void *mem_alloc_frame(size_t sz, size_t align, char *tag, char *file, uint32_t line)
{
    char *format = "[alloc:frame][tag:%s][size:%d][align:%d][file:%s][line:%d]";
    LOG_DEBUG(MEMORY_LOG, format, tag, sz, align, file, line);

    size_t current = atomic_fetch_add_explicit(&glb_frame_mem.offset, sz, memory_order_relaxed);

    LOG_ASSERT(glb_frame_mem.offset < glb_frame_mem.size, "out of frame memory");

    void *ptr = glb_frame_mem.buf + current;
    memset(ptr, 0, sz);
    return ptr;
}

void mem_free_frame(char *tag, char *file, uint32_t line)
{
    char *format = "[free:frame][tag:%s][file:%s][line:%d]";
    LOG_DEBUG(MEMORY_LOG, format, tag, file, line);

    glb_frame_mem.offset = 0;
}

void mem_dump(char *tag, char *file, uint32_t line)
{
    char *format = "[dump][tag:%s][file:%s][line:%d]";
    LOG_DEBUG(MEMORY_LOG, format, tag, file, line);

    if (glb_memory) {
        free(glb_memory);
    }
}
