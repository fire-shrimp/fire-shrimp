

#pragma once

#include <stdint.h>

#define LOG_FATAL(channel_mask, format, ...) write_log(FATAL_LEVEL, channel_mask, format, __VA_ARGS__)
#define LOG_CRITICAL(channel_mask, format, ...) write_log(CRITICAL_LEVEL, channel_mask, format, __VA_ARGS__)
#define LOG_ERROR(channel_mask, format, ...) write_log(ERROR_LEVEL, channel_mask, format, __VA_ARGS__)
#define LOG_WARN(channel_mask, format, ...) write_log(WARN_LEVEL, channel_mask, format, __VA_ARGS__)
#define LOG_INFO(channel_mask, format, ...) write_log(INFO_LEVEL, channel_mask, format, __VA_ARGS__)
#define LOG_DEBUG(channel_mask, format, ...) write_log(DEBUG_LEVEL, channel_mask, format, __VA_ARGS__)

#define NUM_LOG_CHANNELS (6)

enum log_severity {
    FATAL_LEVEL = 0,
    CRITICAL_LEVEL = 10,
    ERROR_LEVEL = 20,
    WARN_LEVEL = 30,
    INFO_LEVEL = 40,
    DEBUG_LEVEL = 50
};

// can only support < 32 log channels
enum log_channels {
    CORE_LOG = 1u << 0,
    MEMORY_LOG = 1u << 1,
    RENDERER_LOG = 1u << 2,
    ANIMATION_LOG = 1u << 3,
    PHYSICS_LOG = 1u << 4,
    USER_LOG = 1u << 5
};

typedef void (*open_log_fn)(void *instance, uint32_t channel_id, char *name);

typedef void (*write_log_fn)(void *instance, uint32_t channel_mask, char *message);

typedef void (*close_log_fn)(void *instance, uint32_t channel_id);

/*
        Dont log errors in logger. Will create deadlock.
*/
typedef struct logger_interface {
    open_log_fn open_fn;
    write_log_fn write_fn;
    close_log_fn close_fn;
} logger_interface;

void init_log(enum log_severity severity);

void set_log_severity(enum log_severity severity);

void add_logger(logger_interface *logger, void *instance);

void write_log(enum log_severity severity, uint32_t channel_mask, char *format, ...);

void shutdown_log(void);
