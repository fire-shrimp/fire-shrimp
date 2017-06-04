
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "config.h"
#include "string-buf.h"

typedef struct file_stats {
    char name[MAX_FILE_NAME + 1];
    char extension[MAX_FILE_EXTENSION + 1];
    size_t size;
} file_stats;

typedef struct directory_stats {
    file_stats *files;
    uint32_t num_files;
} directory_stats;

bool create_path(string_buf *path);

bool get_path_to_executable(string_buf *path);

bool get_working_directory(string_buf *path);

bool set_working_directory(string_buf *path);

bool open_directory(directory_stats *dir, string_buf *path);

void close_directory(directory_stats *dir);
