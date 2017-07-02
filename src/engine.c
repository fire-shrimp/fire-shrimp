
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine.h"
#include "config.h"
#include "command.h"
#include "data.h"
#include "event-queue.h"
#include "execution-graph.h"
#include "file-system.h"
#include "job.h"
#include "job-scheduler.h"
#include "macros.h"
#include "mem.h"

/************ GLOBALS ************/

struct {
    game_data *data;
    job_scheduler scheduler;
    event_queue queue;
} glb_engine;

/************* PUBLIC *************/

void raise_event(uint64_t evt_id, void *args, size_t args_sz)
{
}

void debug_log(enum log_channel channel, const char* format, ...)
{
}

/************** MAIN **************/

static void boot_engine(command_args *args)
{
    if(args->working_directory != NULL) {
        bool result = set_working_directory(args->working_directory);

        CHECK(result, "failed to set working directory to \"%s\"", args->working_directory);
    }

    glb_engine.data = load_data();

    init_scheduler(&glb_engine.scheduler, WORKER_THREADS);

#ifdef DEBUG_BUILD
    //glb_engine.socket_server = create_socket_server(DEFAULT_DEBUG_PORT);
    //glb_engine.server_task = pin_task(glb_engine.scheduler, open_server, glb_engine.socket_server);
#endif
}

static void game_routine(job_scheduler *scheduler, job_item *job)
{
    execution_graph *graph = &(glb_engine.data->graph);
    job_batch *root = NULL;
    job_batch *previous = NULL;
    job_batch *current = NULL;

    for(uint32_t i = 0; i < graph->capacity; i++){
        current = create_job_batch(graph->nodes[i].count);

        for(uint32_t j = 0; j < graph->nodes[i].count; j++){
            add_job(current, 0, execute_unit, &graph->nodes[i].units[j]);
        }

        if(root == NULL){
            root = current;
        }

        if(previous != NULL){
            add_continuation(previous, current);
        }

        previous = current;
    }

    submit_batch(scheduler, root);
    wait_for_batch(scheduler, current);
}

static void render_routine(job_scheduler *scheduler, job_item *job)
{
    //LOG_DEBUG(CORE_LOG, "render job");

    uint32_t g = 0;

    for (uint32_t i = 0; i < 10000; i++) {
        g++;
    }
}

static void device_routine(job_scheduler *scheduler, job_item *job)
{
    //LOG_DEBUG(CORE_LOG, "device job");

    uint32_t g = 0;

    for (uint32_t i = 0; i < 10000; i++) {
        g++;
    }
}

static void tick()
{
    //do timing

    job_batch *batch = create_job_batch(3);
    add_job(batch, 0, game_routine, NULL);
    add_job(batch, 0, render_routine, NULL);
    add_job(batch, 0, device_routine, NULL);

    submit_batch(&glb_engine.scheduler, batch);
    wait_for_batch(&glb_engine.scheduler, batch);
}

static bool process_events()
{
    // for(uint32_t i = 0; i < glb_engine.event_queue.count; i++){

    //     event *evt = get_at_index_lock_free_buf(&glb_engine.event_queue, i);

    //     if(evt->id == SYS_EVENT_QUIT){
    //         return false;
    //     } else if(evt->id < MAX_SYS_EVENTS){
    //         //handle sys event
    //     } else {
    //         //handle user event
    //     }
    // }

    return true;
}

static void shutdown_engine()
{
    join_threads(&glb_engine.scheduler);
}

int main(int argc, char **argv)
{
    command_args args = {};
    set_default_args(&args);
    parse_command_args(&args, argc, argv);

    mem_init();

    boot_engine(&args);

    do {
        mem_free(FRAME_MEM, 0, 0, "frame mem");
        tick();
    } while(process_events());

    shutdown_engine();

    mem_dump();

    return 0;
}

