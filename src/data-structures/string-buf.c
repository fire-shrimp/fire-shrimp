
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "string-buf.h"
#include "mem.h"

string_buf *create_string_buf(enum mem_arena arena, size_t sz)
{
    string_buf *buf = mem_alloc(arena, sizeof(string_buf) + sz, 0, "string_buf");
    buf->arena = arena;
    buf->buf = (char *)((uint8_t*)buf + sizeof(string_buf));
    buf->size = sz;

    return buf;
}

void destroy_string_buf(string_buf *buf)
{
    mem_free(buf->arena, buf, "string_buf");
}
