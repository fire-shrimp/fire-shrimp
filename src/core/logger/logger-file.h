
#pragma once

#include <stdint.h>

enum log_severity;

void log_file_open_channel(uint32_t channel_id, char *channel_name);

void log_file(uint32_t channel_mask, char *message);

void log_file_close_channel(uint32_t channel_id);
