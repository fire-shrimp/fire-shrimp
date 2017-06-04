
#include <stdarg.h>

#include "config.h"
#include "log.h"
#include "macros.h"
#include "string-util.h"
#include "thread-id.h"
#include "tinycthread.h"

struct {
    struct {
        logger_interface *interface;
        void *instance;
    } loggers[MAX_LOGGERS];

    uint8_t num_loggers;
    enum log_severity severity;
    char buf[MAX_LOG_ENTRY_SIZE + 1];
    mtx_t mutex;
} glb_log;

void init_log(enum log_severity severity)
{
    CHECK(mtx_init(&glb_log.mutex, mtx_plain));

    set_log_severity(severity);
}

void set_log_severity(enum log_severity severity)
{
    glb_log.severity = severity;
}

void add_logger(logger_interface *interface, void *instance)
{
    if (glb_log.num_loggers >= MAX_LOGGERS) {
        return;
    }

    glb_log.loggers[glb_log.num_loggers].interface = interface;
    glb_log.loggers[glb_log.num_loggers].instance = instance;

    //open all channels
    glb_log.loggers[glb_log.num_loggers].interface->open_fn(instance, CORE_LOG, "core.log");
    glb_log.loggers[glb_log.num_loggers].interface->open_fn(instance, MEMORY_LOG, "mem.log");

    glb_log.num_loggers++;
}

void write_log(enum log_severity severity, uint32_t channel_mask, char *format, ...)
{
    if (glb_log.num_loggers < 1) {
        return;
    }

    //ignore the event if not severe enough
    if (severity <= glb_log.severity) {
        CHECK(mtx_lock(&glb_log.mutex));

        size_t dest_sz = MAX_LOG_ENTRY_SIZE + 1;
        size_t count = MAX_LOG_ENTRY_SIZE;

        //write entry header
        uint32_t thrd_id = get_thrd_id();
        int32_t offset = write_string_format(glb_log.buf, dest_sz, count, "[%s][thread id:%d]", "SEV", thrd_id);

        //write entry to buf
        va_list args;
        va_start(args, format);
        write_string_args(glb_log.buf + offset, dest_sz, count, format, args);
        va_end(args);

        //write to loggers
        for (uint32_t i = 0; i < glb_log.num_loggers; i++) {
            void *instance = glb_log.loggers[i].instance;
            glb_log.loggers[i].interface->write_fn(instance, channel_mask, glb_log.buf);
        }

        CHECK(mtx_unlock(&glb_log.mutex));
    }
}

void shutdown_log()
{
    for (uint32_t i = 0; i < glb_log.num_loggers; i++) {
        for (uint32_t j = 0; j < NUM_LOG_CHANNELS; j++) {
            void *instance = glb_log.loggers[i].instance;
            glb_log.loggers[i].interface->close_fn(instance, j);
        }
    }
}
