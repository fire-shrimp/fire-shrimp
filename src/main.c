#include "compiler-guard.h"
#include "config.h"
#include "data.h"
#include "engine-jobs.h"
#include "engine.h"
#include "file-system.h"
#include "job-scheduler.h"
#include "job.h"
#include "log-file.h"
#include "log.h"
#include "macros.h"
#include "mem.h"
#include "string-util.h"

int main(int argc, char **argv)
{
    // allocate all memory
    MEM_INIT(STATIC_MEM, HEAP_MEM, FRAME_MEM);

    string_buf *path = create_string_buf(MAX_FILE_PATH + 1);

    // set working directory
    CHECK(get_path_to_executable(path));
    CHECK(set_working_directory(path));

    destroy_string_buf(path);

    // start loggers
    init_log(DEBUG_LEVEL);

    file_logger logger = {
        .interface.open_fn = open_file_log,
        .interface.write_fn = write_file_log,
        .interface.close_fn = close_file_log,
        .channels = {}};

    add_logger(&(logger.interface), &(logger));

    //boot engine
    //game_engine engine = {}
    //init_engine(&engine);

    // load game
    game_data *data = load_data();

    // create scheduler
    job_scheduler scheduler = {};
    init_scheduler(&scheduler, 7);

    for (uint32_t i = 0; i < 10; i++) {
        // do timing

        job_item *root_job = create_job(0, NULL, NULL, NULL);

        job_item *game_job = create_job(0, game_routine, root_job, NULL);
        add_continuation(root_job, game_job);

        job_item *render_job = create_job(0, render_routine, root_job, NULL);
        add_continuation(root_job, render_job);

        job_item *device_job = create_job(0, device_routine, root_job, NULL);
        add_continuation(root_job, device_job);

        submit_job(&scheduler, root_job);
        wait_for_job(&scheduler, root_job);
        sync_threads(&scheduler);

        MEM_FREE_FRAME("end Frame");
    }

    join_threads(&scheduler);
    MEM_DUMP("exit");
    shutdown_log();

    return 0;
}
