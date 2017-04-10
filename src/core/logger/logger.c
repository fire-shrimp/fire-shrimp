
#include <stdarg.h>

#include "logger.h"
#include "config.h"
#include "macros.h"
#include "tinycthread.h"
#include "thread-id.h"
#include "char-buf.h"

struct log
{
	struct logger loggers[MAX_LOGGERS];
	uint8_t num_loggers;
	enum log_severity severity;
	char buf[MAX_LOG_ENTRY_SIZE + 1];
	mtx_t mutex;
};

static struct log glb_log;

void log_init(enum log_severity severity)
{
	CHECK(mtx_init(&glb_log.mutex, mtx_plain));

	log_set_severity_level(severity);
}

void log_set_severity_level(enum log_severity severity)
{
	glb_log.severity = severity;
}

void log_add_logger(logger_open_channel_fn open_channel_fn, logger_entry_fn log_fn, logger_close_channel_fn close_channel_fn)
{
	if(glb_log.num_loggers >= MAX_LOGGERS)
		return;

	glb_log.loggers[glb_log.num_loggers].open_channel_fn = open_channel_fn;
	glb_log.loggers[glb_log.num_loggers].log_fn = log_fn;
	glb_log.loggers[glb_log.num_loggers].close_channel_fn = close_channel_fn;

	//open all channels
	glb_log.loggers[glb_log.num_loggers].open_channel_fn(CORE_LOG, "core.log");
	glb_log.loggers[glb_log.num_loggers].open_channel_fn(MEMORY_LOG, "mem.log");

	glb_log.num_loggers++;
}

void log_entry(enum log_severity severity, char *severity_name, uint32_t channel_mask, char *format, ...)
{
	if(glb_log.num_loggers < 1)
		return;

	//ignore the event if not severe enough
	if (severity <= glb_log.severity)
	{
		CHECK(mtx_lock(&glb_log.mutex));

		//write entry header
		uint32_t thrd_id = get_thrd_id();
		int32_t offset = write_char_buf_format(glb_log.buf, MAX_LOG_ENTRY_SIZE + 1, MAX_LOG_ENTRY_SIZE, "[%s][thread id:%d]", severity_name, thrd_id );

		//write entry
		va_list args;
		va_start(args, format);
		write_char_buf_args(glb_log.buf + offset, MAX_LOG_ENTRY_SIZE + 1, MAX_LOG_ENTRY_SIZE, format, args);
		va_end(args);

		for (uint32_t i = 0; i < glb_log.num_loggers; i++)
			glb_log.loggers[i].log_fn(channel_mask, glb_log.buf);

		CHECK(mtx_unlock(&glb_log.mutex));
	}
}

void log_shutdown()
{
	for(uint32_t i = 0; i < glb_log.num_loggers; i++)
	{
		for(uint32_t j = 0; j < NUM_LOG_CHANNELS; j++)
			glb_log.loggers[i].close_channel_fn(j);
	}
}
