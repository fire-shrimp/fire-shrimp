

#include "config.h"
#include "file-system.h"
#include "engine-jobs.h"
#include "job.h"
#include "job-scheduler.h"
#include "mem.h"
#include "logger.h"
#include "logger-file.h"
#include "macros.h"
#include "game.h"
#include "char-buf.h"

int main(int argc, char **argv)
{
	//allocate all memory
	MEM_INIT(STATIC_MEM, HEAP_MEM, FRAME_MEM);

	struct char_buf *path = create_char_buf(MAX_FILE_PATH + 1);

	//set working directory
	CHECK(get_path_to_executable(path));
	CHECK(set_working_directory(path));

	//start loggers
	log_init(DEBUG_LEVEL);
	log_add_logger(log_file_open_channel, log_file, log_file_close_channel);

	append_char_buf(path->buf, path->size, "\\..\\data");

	//load game
	struct game_data game = {};
	load_game(&game, path);

	destroy_char_buf(path);

	//create scheduler
	struct job_scheduler scheduler = {};
	scheduler_init(&scheduler, 7);

	for(uint32_t i = 0; i < 10; i++)
	{
		//do timing

		struct job_item *frame_root_job = job_create(0, NULL, NULL, NULL);

		struct job_item *game_logic_job = job_create(0, engine_game_logic, frame_root_job, NULL);
		job_add_continuation(frame_root_job, game_logic_job);

		struct job_item *render_front_end_job = job_create(0, engine_render_front_end, frame_root_job, NULL);
		job_add_continuation(frame_root_job, render_front_end_job);

		struct job_item *render_back_end_job = job_create(0, engine_render_back_end, frame_root_job, NULL);
		job_add_continuation(frame_root_job, render_back_end_job);

		scheduler_submit(&scheduler, frame_root_job);
		scheduler_wait(&scheduler, frame_root_job);
		scheduler_sync(&scheduler);

		MEM_FREE_FRAME("end Frame");
	}

	scheduler_join(&scheduler);
	MEM_DUMP("exit");
	log_shutdown();

	return 0;
}
