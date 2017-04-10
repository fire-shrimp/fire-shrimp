
#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "char-buf.h"

struct directory_stats
{
	struct file_stats *files;
	uint32_t num_files;
};

struct file_stats
{
	char name[MAX_FILE_NAME + 1];
	char extension[MAX_FILE_EXTENSION + 1];
	size_t size;
};

bool create_path(struct char_buf *path);

bool get_path_to_executable(struct char_buf *path);

bool get_working_directory(struct char_buf *path);

bool set_working_directory(struct char_buf *path);

bool open_directory(struct directory_stats *dir, struct char_buf *path);

void close_directory(struct directory_stats *dir);
