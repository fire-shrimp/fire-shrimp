#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "compiler-guard.h"
#include "config.h"
#include "data-type.h"
#include "data.h"
#include "engine.h"
#include "error.h"
#include "event-handler.h"
#include "event.h"
#include "execution-graph.h"
#include "file-system.h"
#include "job-scheduler.h"
#include "job.h"
#include "lock-free-buf.h"
#include "mem.h"

/************ GLOBALS ************/

static struct {
    uint64_t frame;
    game_data *data;
    job_scheduler scheduler;
    lock_free_buf evt_queue;
} glb_engine;

/************* PUBLIC *************/
event *raise_event()
{
    return push_lock_free_buf(&glb_engine.evt_queue);
}

void debug_log(enum log_channel channel, const char *format, ...)
{
}

/************** MAIN **************/

static void boot_engine(command_args *args)
{
    mem_init();

    if (args->working_directory != NULL) {
        bool result = set_working_directory(args->working_directory);
        CHECK(result, "failed to set working directory to \"%s\"", args->working_directory);
    }

    glb_engine.frame = 0;
    glb_engine.data = load_data();

    init_event_handler(&glb_engine.data->evt_handlers[SYS_FRAME - NUM_NATIVE_HANLDERS], SYS_FRAME, UINT64_TYPE);
    init_scheduler(&glb_engine.scheduler, WORKER_THREADS);
    init_lock_free_buf(&glb_engine.evt_queue, MAX_EVENTS, sizeof(event));
}

static void game_routine(job_scheduler *scheduler, job_item *job)
{
    printf("game\n");
    // execution_graph *graph = &(glb_engine.data->graph);
    // job_batch *root = NULL;
    // job_batch *previous = NULL;
    // job_batch *current = NULL;

    // for (uint32_t i = 0; i < graph->capacity; i++) {
    //     current = create_job_batch(graph->nodes[i].count);

    //     for (uint32_t j = 0; j < graph->nodes[i].count; j++) {
    //         add_job(current, 0, execute_unit, &graph->nodes[i].units[j]);
    //     }

    //     if (root == NULL) {
    //         root = current;
    //     }

    //     if (previous != NULL) {
    //         add_continuation(previous, current);
    //     }

    //     previous = current;
    // }

    // submit_batch(scheduler, root);
    //// wait_for_batch(scheduler, current);
}

static void render_routine(job_scheduler *scheduler, job_item *job)
{
    printf("render\n");
    //LOG_DEBUG(CORE_LOG, "render job");

    // uint32_t g = 0;

    // for (uint32_t i = 0; i < 10000; i++) {
    //     g++;
    // }
}

static void device_routine(job_scheduler *scheduler, job_item *job)
{
    printf("device\n");
    //LOG_DEBUG(CORE_LOG, "device job");

    //uint32_t g = 0;

    // for (uint32_t i = 0; i < 10000; i++) {
    //     g++;
    // }
}

static void tick()
{
    mem_free(FRAME_MEM, NULL, "frame");

    job_batch *batch = create_job_batch(3, NULL);
    add_job(batch, 0, game_routine, NULL);
    add_job(batch, 0, render_routine, NULL);
    add_job(batch, 0, device_routine, NULL);

    submit_batch(&glb_engine.scheduler, batch);
    wait_for_batch(&glb_engine.scheduler, batch);
}

static void raise_sys_events()
{
    event *evt = raise_event();
    evt->id = SYS_FRAME;
    evt->arg.type = UINT64_TYPE;
    evt->arg.uint64_value = ++glb_engine.frame;
}

static bool process_events()
{
    for (uint32_t i = 0; i < glb_engine.data->num_evt_handlers; i++) {
        reset_event_handler(&glb_engine.data->evt_handlers[i]);
    }

    raise_sys_events();

    for (uint32_t i = 0; i < glb_engine.evt_queue.count; i++) {
        event *evt = get_at_index_lock_free_buf(&glb_engine.evt_queue, i);

        if (evt->id == SYS_QUIT) {
            return false;
        } else if (evt->id == SYS_LOAD_LEVEL) {
            //load_level
        } else {
            uint32_t num_handlers = glb_engine.data->num_evt_handlers + NUM_NATIVE_HANLDERS;

            ASSERT(evt->id < num_handlers, "invalid event id");

            process_event(&glb_engine.data->evt_handlers[evt->id - NUM_NATIVE_HANLDERS], evt);
        }
    }

    reset_lock_free_buf(&glb_engine.evt_queue);

    return true;
}

static void shutdown_engine()
{
    join_threads(&glb_engine.scheduler);
    mem_dump();
}

int main(int argc, char **argv)
{
    command_args args = {};
    set_default_args(&args);
    parse_command_args(&args, argc, argv);

    boot_engine(&args);

    while (process_events()) {
        tick();
    }

    shutdown_engine();

    return 0;
}
