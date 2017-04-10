
#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MEM_INIT(static_sz, heap_sz, frame_sz) (mem_init(static_sz, heap_sz, frame_sz))
#define MALLOC_STATIC(size, align, tag) (mem_alloc_static(size, align, tag, __FILE__, __LINE__))
#define MALLOC_HEAP(size, align, tag) (mem_alloc_heap(size, align, tag, __FILE__, __LINE__))
#define MEM_FREE_HEAP(ptr)
#define MALLOC_FRAME(size, align, tag) (mem_alloc_frame(size, align, tag, __FILE__, __LINE__))
#define MEM_FREE_FRAME(tag) (mem_free_frame(tag, __FILE__, __LINE__))
#define MEM_DUMP(tag) (mem_dump(tag, __FILE__, __LINE__))

void mem_init(size_t static_sz, size_t heap_sz, size_t frame_sz);
void *mem_alloc_static(size_t size, size_t align, char *tag, char *file, uint32_t line);
void *mem_alloc_heap(size_t size, size_t align, char *tag, char *file, uint32_t line);
void mem_free_heap();
void *mem_alloc_frame(size_t size, size_t align, char *tag, char *file, uint32_t line);
void mem_free_frame(char *tag, char *file, uint32_t line);
void mem_dump(char *tag, char *file, uint32_t line);

