
#pragma once

#include <stdbool.h>
#include <stdio.h>

bool open_file(FILE **fp, char *path, char *flags);

void read_file(FILE *fp, void *buf, size_t element_size, size_t element_count);

bool load_file(char *path, uint8_t *buf, size_t buf_size);

void write_file_format(FILE *fp, char *format, ...);

void close_file(FILE *fp);

