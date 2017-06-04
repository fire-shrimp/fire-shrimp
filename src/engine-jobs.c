
#include "engine-jobs.h"
#include "config.h"
#include "job-scheduler.h"
#include "job.h"
#include "log.h"
#include "mem.h"

void game_routine()
{
    LOG_DEBUG(CORE_LOG, "Game job");

    uint32_t g = 0;

    for (uint32_t i = 0; i < 10000; i++) {
        g++;
    }
}

void render_routine()
{
    LOG_DEBUG(CORE_LOG, "Render job");

    uint32_t g = 0;

    for (uint32_t i = 0; i < 10000; i++) {
        g++;
    }
}

void device_routine()
{
    LOG_DEBUG(CORE_LOG, "Device job");

    uint32_t g = 0;

    for (uint32_t i = 0; i < 10000; i++) {
        g++;
    }
}
