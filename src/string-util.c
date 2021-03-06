
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "string-util.h"

bool compare_string(char *str_1, char *str_2)
{
    int32_t result = strcmp(str_1, str_2);

    return (result == 0);
}

int32_t write_string_format(char *dest, size_t dest_sz, size_t count, char *format, ...)
{
    va_list args;
    va_start(args, format);
    int32_t result = vsnprintf_s(dest, dest_sz, count, format, args);
    va_end(args);

    CHECK(result >= 0, "write to char buf failed");

    return result;
}

int32_t write_string_args(char *dest, size_t dest_sz, size_t count, char *format, va_list args)
{
    int32_t result = vsnprintf_s(dest, dest_sz, count, format, args);

    CHECK(result >= 0, "write to char buf failed");

    return result;
}

void write_string_count(char *dest, size_t dest_sz, char *src, size_t count)
{
    errno_t err = strncpy_s(dest, dest_sz, src, count);

    CHECK(err == 0, "write to char buf failed");
}

void append_string(char *dest, size_t dest_sz, char *src)
{
    errno_t err = strcat_s(dest, dest_sz, src);

    CHECK(err == 0, "append char buf failed");
}

bool starts_with(char *str_1, char *str_2, size_t count)
{
    return (strncmp(str_1, str_2, count) == 0);
}

size_t length(char *str, size_t count)
{
    return strnlen_s( str, count );
}
