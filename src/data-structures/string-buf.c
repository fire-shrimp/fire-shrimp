
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mem.h"
#include "string-buf.h"

string_buf *create_static_string_buf(size_t sz)
{
    void *ptr = MALLOC_STATIC(sizeof(string_buf) + sz, 0, "create static string");

    string_buf *buf = ptr;
    buf->buf = (char *)ptr + sizeof(string_buf);
    buf->size = sz;

    return buf;
}

string_buf *create_string_buf(size_t sz)
{
    void *ptr = MALLOC_HEAP(sizeof(string_buf) + sz, 0, "create string");

    string_buf *buf = ptr;
    buf->buf = (char *)ptr + sizeof(string_buf);
    buf->size = sz;

    return buf;
}

void destroy_string_buf(string_buf *buf)
{
    MEM_FREE_HEAP(buf);
}
