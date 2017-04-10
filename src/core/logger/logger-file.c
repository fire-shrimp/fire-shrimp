
#include <stdint.h>

#include "logger-file.h"
#include "logger.h"
#include "config.h"
#include "macros.h"
#include "tinycthread.h"
#include "char-buf.h"
#include "file.h"

struct log_file_channel
{
	char file_name[MAX_LOG_FILE_NAME_SIZE + 1];
	FILE *fp;
};

static struct log_file_channel glb_channels[NUM_LOG_CHANNELS];

void log_file_open_channel(uint32_t channel_id, char *channel_name)
{
	if(channel_id >= NUM_LOG_CHANNELS || glb_channels[channel_id].fp)
		return;

	write_char_buf_count(glb_channels[channel_id].file_name, MAX_LOG_FILE_NAME_SIZE + 1, channel_name, MAX_LOG_FILE_NAME_SIZE);

	if(!open_file(&glb_channels[channel_id].fp, channel_name, "wb"))
		return;

	write_file_format(glb_channels[channel_id].fp, "======= %s ======\n", channel_name);
}

void log_file(uint32_t channel_mask, char *message)
{
	for(uint32_t i = 0; i < NUM_LOG_CHANNELS; i++)
	{
		if(((1u << i) & channel_mask) && glb_channels[i].fp)
			write_file_format(glb_channels[i].fp, "%s\n", message);
	}
}

void log_file_close_channel(uint32_t channel_id)
{
	if(channel_id >= NUM_LOG_CHANNELS)
		return;

	if (glb_channels[channel_id].fp)
		close_file(glb_channels[channel_id].fp);
}


