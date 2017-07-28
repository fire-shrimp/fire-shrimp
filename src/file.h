
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define ZP_MAKEFOURCC(ch0, ch1, ch2, ch3) (((uint32_t)(ch0) | ((uint32_t)(ch1) << 8) | ((uint32_t)(ch2) << 16) | ((uint32_t)(ch3) << 24)))

bool open_file(FILE **fp, char *path, char *flags);

size_t read_file(FILE *fp, void *buf, size_t element_sz, size_t element_count, bool check_eof);

size_t read_string_from_file(FILE *fp, char *dest, size_t dest_sz, size_t count, bool check_eof);

void write_file_format(FILE *fp, char *format, ...);

void close_file(FILE *fp);
