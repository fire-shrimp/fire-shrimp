
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "config.h"

enum mem_arena{
    STATIC_MEM = 0,
    FRAME_MEM = 1,
    HEAP_MEM = 2,
#ifdef DEBUG_BUILD
    DEBUG_STATIC_MEM = 3,
    DEBUG_FRAME_MEM = 4,
    DEBUG_HEAP_MEM = 5,
    NUM_ARENAS = 6
#else
    NUM_ARENAS = 3
#endif
};

void mem_init();

void *mem_alloc(enum mem_arena arena, size_t sz, size_t align, const char *tag);

void mem_free(enum mem_arena arena, void *ptr, const char *tag);

void mem_dump();
