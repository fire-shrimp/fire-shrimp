
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "char-buf.h"
#include "config.h"

typedef struct file_stats {
    char name[MAX_FILE_NAME + 1];
    char extension[MAX_FILE_EXTENSION + 1];
    size_t size;
} file_stats;

typedef struct directory_stats {
    file_stats *files;
    uint32_t num_files;
} directory_stats;

bool create_path(char_buf *path);

bool get_path_to_executable(char_buf *path);

bool get_working_directory(char_buf *path);

bool set_working_directory(char_buf *path);

bool open_directory(directory_stats *dir, char_buf *path);

void close_directory(directory_stats *dir);
