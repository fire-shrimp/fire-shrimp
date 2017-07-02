#pragma once

#include "allocator.h"

enum mem_arena;

typedef struct string_buf {
    enum mem_arena arena;
    char *buf;
    size_t size;
} string_buf;

string_buf *create_string_buf(enum mem_arena arena, size_t sz);

void destroy_string_buf(string_buf *buf);
