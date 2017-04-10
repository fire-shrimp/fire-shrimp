

#pragma once

#include <stdint.h>

#define LOG_FATAL(channel_mask, format, ...) log_entry(FATAL_LEVEL, "FATAL", channel_mask, format, __VA_ARGS__)
#define LOG_CRITICAL(channel_mask, format, ...) log_entry(CRITICAL_LEVEL, "CRITICAL",channel_mask, format, __VA_ARGS__)
#define LOG_ERROR(channel_mask, format, ...) log_entry(ERROR_LEVEL, "ERROR", channel_mask, format, __VA_ARGS__)
#define LOG_WARN(channel_mask, format, ...) log_entry(WARN_LEVEL,"WARN", channel_mask, format, __VA_ARGS__)
#define LOG_INFO(channel_mask, format, ...) log_entry(INFO_LEVEL, "INFO", channel_mask, format, __VA_ARGS__)
#define LOG_DEBUG(channel_mask, format, ...) log_entry(DEBUG_LEVEL, "DEBUG", channel_mask, format, __VA_ARGS__)

#define NUM_LOG_CHANNELS (6)

enum log_severity
{
	FATAL_LEVEL = 0,
	CRITICAL_LEVEL = 10,
	ERROR_LEVEL = 20,
	WARN_LEVEL = 30,
	INFO_LEVEL = 40,
	DEBUG_LEVEL = 50
};

//can only support < 32 log channels
enum log_channels
{
	CORE_LOG = 1u << 0,
	MEMORY_LOG = 1u << 1,
	RENDERER_LOG = 1u << 2,
	ANIMATION_LOG = 1u << 3,
	PHYSICS_LOG = 1u << 4,
	USER_LOG = 1u << 5
};

typedef void(*logger_open_channel_fn)(uint32_t channel_id, char *name);

typedef void(*logger_entry_fn)(uint32_t channel_mask, char *message);

typedef void(*logger_close_channel_fn)(uint32_t channel_id);

/*
	Dont log errors in logger. Will create deadlock.
*/
struct logger
{
	logger_open_channel_fn open_channel_fn;
	logger_entry_fn log_fn;
	logger_close_channel_fn close_channel_fn;
};

void log_init(enum log_severity severity);
void log_set_severity_level(enum log_severity severity);
void log_add_logger(logger_open_channel_fn open_channel_fn, logger_entry_fn log_fn, logger_close_channel_fn close_channel_fn);
void log_entry(enum log_severity severity, char *severity_name, uint32_t channel_mask, char *format, ...);
void log_shutdown(void);
