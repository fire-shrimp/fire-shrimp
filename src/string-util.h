#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

bool compare_string(char *string_1, char *string_2);

int32_t write_string_format(char *dest, size_t dest_sz, size_t count, char *format, ...);

int32_t write_string_args(char *dest, size_t dest_sz, size_t count, char *format, va_list args);

void write_string_count(char *dest, size_t dest_sz, char *src, size_t count);

void append_string(char *dest, size_t dest_sz, char *src);

bool starts_with(char *string_1, char *string_2, size_t count);
