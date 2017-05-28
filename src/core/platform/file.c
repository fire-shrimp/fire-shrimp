
#include <stdarg.h>
#include <stdint.h>

#include "file.h"
#include "log.h"
#include "macros.h"

bool open_file(FILE **fp, char *path, char *flags)
{
    errno_t err = fopen_s(fp, path, flags);

    return (err == 0);
}

void read_file(FILE *fp, void *buf, size_t element_size, size_t element_count)
{
    LOG_ASSERT(fp, "fp is null");
    LOG_ASSERT(buf, "buffer is null");

    size_t result = fread(buf, element_size, element_count, fp);
    int32_t eof = feof(fp);
    int32_t file_err = ferror(fp);

    bool success = (result == element_count || eof || !file_err);

    LOG_CHECK(success, "read file failed with code %d", file_err);
}

bool load_file(char *path, uint8_t *buf, size_t buf_size)
{
    FILE *fp;
    errno_t err = fopen_s(&fp, path, "rb");

    if (err != 0) {
        return false;
    }

    size_t result = fread(buf, sizeof(uint8_t), buf_size, fp);
    int32_t eof = feof(fp);
    int32_t file_err = ferror(fp);

    return !(result == buf_size && !eof && !file_err);
}

void write_file_format(FILE *fp, char *format, ...)
{
    LOG_ASSERT(fp, "fp is null");

    va_list args;
    va_start(args, format);
    int32_t result = vfprintf_s(fp, format, args);
    va_end(args);

    LOG_CHECK(result >= 0, "write to file failed");

    result = fflush(fp);

    LOG_CHECK(result >= 0, "flush file failed");
}

void close_file(FILE *fp)
{
    LOG_ASSERT(fp, "file is null");

    int32_t result = fclose(fp);

    LOG_CHECK(result == 0, "close file failed");
}
