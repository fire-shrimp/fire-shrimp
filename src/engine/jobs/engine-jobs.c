
#include "engine-jobs.h"
#include "config.h"
#include "job-scheduler.h"
#include "job.h"
#include "mem.h"
#include "logger.h"

void engine_game_logic()
{
	LOG_DEBUG(CORE_LOG, "Game logic job");

	uint32_t g = 0;
	for(uint32_t i = 0; i < 10000; i++)
		g++;

}

void engine_render_front_end()
{
	LOG_DEBUG(CORE_LOG, "Render front end job");

	uint32_t g = 0;
	for(uint32_t i = 0; i < 10000; i++)
		g++;
}

void engine_render_back_end()
{
	LOG_DEBUG(CORE_LOG, "Render back end job");

	uint32_t g = 0;
	for(uint32_t i = 0; i < 10000; i++)
		g++;
}
