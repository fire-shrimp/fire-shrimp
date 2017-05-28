
#pragma once

#include <stdint.h>

#include "config.h"
#include "file.h"
#include "log.h"

enum log_severity;

typedef struct file_channel {
    char file_name[MAX_LOG_FILE_NAME_SIZE + 1];
    FILE *fp;
} file_channel;

typedef struct file_log {
    file_channel channels[NUM_LOG_CHANNELS];
} file_log;

void open_file_log(void *instance, uint32_t channel_id, char *channel_name);

void write_file_log(void *instance, uint32_t channel_mask, char *message);

void close_file_log(void *instance, uint32_t channel_id);
