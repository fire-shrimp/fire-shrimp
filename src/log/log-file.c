
#include <stdint.h>

#include "file.h"
#include "log-file.h"
#include "macros.h"
#include "string-util.h"
#include "tinycthread.h"
#include "util.h"

void open_file_log(void *instance, uint32_t channel_id, char *channel_name)
{
    file_logger *logger = instance;

    int32_t index = least_significant_bit(channel_id);

    if (index < 0 || index >= NUM_LOG_CHANNELS || logger->channels[index].fp) {
        return;
    }

    char *dest = logger->channels[index].file_name;
    size_t dest_sz = MAX_LOG_FILE_NAME_SIZE + 1;
    size_t count = MAX_LOG_FILE_NAME_SIZE;
    write_string_count(dest, dest_sz, channel_name, count);

    if (!open_file(&logger->channels[index].fp, channel_name, "wb")) {
        return;
    }

    write_file_format(logger->channels[index].fp, "======= %s ======\n", channel_name);
}

void write_file_log(void *instance, uint32_t channel_mask, char *message)
{
    file_logger *logger = instance;

    for (uint32_t i = 0; i < NUM_LOG_CHANNELS; i++) {
        if (((1u << i) & channel_mask) && logger->channels[i].fp) {
            write_file_format(logger->channels[i].fp, "%s\n", message);
        }
    }
}

void close_file_log(void *instance, uint32_t channel_id)
{
    file_logger *logger = instance;

    if (channel_id >= NUM_LOG_CHANNELS) {
        return;
    }

    if (logger->channels[channel_id].fp) {
        close_file(logger->channels[channel_id].fp);
    }
}
