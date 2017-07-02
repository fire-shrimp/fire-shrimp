#include <stdarg.h>
#include <stdint.h>

#include "file.h"
#include "macros.h"
#include "math-util.h"

bool open_file(FILE **fp, char *path, char *flags)
{
    errno_t err = fopen_s(fp, path, flags);

    return (err == 0);
}

size_t read_file(FILE *fp, void *buf, size_t element_sz, size_t element_count, bool check_eof)
{
    ASSERT(fp, "fp is null");
    ASSERT(buf, "buffer is null");

    size_t total_read = fread(buf, element_sz, element_count, fp);
    int32_t eof = feof(fp);
    int32_t err = ferror(fp);

    CHECK(!check_eof || !eof, "read file failed with unexpected EOF")
    CHECK((total_read == element_count || eof || !err), "read file failed with code %d", err);

    return total_read;
}

size_t read_string_from_file(FILE *fp, char *dest, size_t dest_sz, size_t count, bool check_eof)
{
    ASSERT(fp, "fp is null");
    ASSERT(dest, "dest is null");
    ASSERT(dest_sz >= 1, "dest_sz is too small. Must be at least 1 byte");

    size_t read_max = min_size(dest_sz - 1, count);
    size_t total_read = 0;
    int32_t c = 0;

    while (total_read < read_max && ((c = fgetc(fp)) != EOF) && c != '\0') {
        dest[total_read] = c;
        total_read++;
    }

    //null terminate the string
    dest[total_read] = '\0';

    int32_t eof = feof(fp);
    int32_t err = ferror(fp);

    CHECK(!check_eof || !eof, "read file failed with unexpected EOF")
    CHECK((total_read == read_max || eof || !err), "read file failed with code %d", err);

    return total_read;
}

void write_file_format(FILE *fp, char *format, ...)
{
    ASSERT(fp, "fp is null");

    va_list args;
    va_start(args, format);
    int32_t result = vfprintf_s(fp, format, args);
    va_end(args);

    CHECK(result >= 0, "write to file failed");

    result = fflush(fp);

    CHECK(result >= 0, "flush file failed");
}

void close_file(FILE *fp)
{
    ASSERT(fp, "fp is null");

    int32_t result = fclose(fp);

    CHECK(result == 0, "close file failed");
}
