#pragma once

typedef struct string_buf {
    char *buf;
    size_t size;
} string_buf;


string_buf *create_static_string_buf(size_t sz);

string_buf *create_string_buf(size_t sz);

void destroy_string_buf(string_buf *buf);
