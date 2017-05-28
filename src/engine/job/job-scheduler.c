
#include <stdlib.h>

#include "job-scheduler.h"
#include "job.h"
#include "macros.h"
#include "mem.h"
#include "thread-id.h"

struct thrd_start_args {
    job_scheduler *scheduler;
    uint32_t queue_id;
};

int32_t worker_thread(void *args);

void do_work(job_scheduler *scheduler);

job_item *find_work(job_scheduler *scheduler);

_Thread_local uint32_t glb_queue_id;

void init_scheduler(job_scheduler *scheduler, uint32_t num_threads)
{
    LOG_ASSERT(num_threads < MAX_WORKER_THREADS, "too many threads requested");

    scheduler->num_threads = num_threads;
    atomic_store_explicit(&scheduler->join_workers, false, memory_order_relaxed);

    size_t args_sz = sizeof(struct thrd_start_args) * num_threads;
    struct thrd_start_args *args = MALLOC_STATIC(args_sz, 0, "Job scheduler thread args");

    for (uint32_t i = 0; i < num_threads + 1; i++) {
        init_job_queue(&scheduler->queues[i], MAX_JOBS_PER_QUEUE);
    }

    // main thread uses first queue
    glb_queue_id = 0;

    for (uint32_t i = 0; i < num_threads; i++) {
        args[i].scheduler = scheduler;
        args[i].queue_id = i + 1;

        LOG_CHECK(thrd_create(&scheduler->threads[i], worker_thread, (void *)&(args[i])),
            "thrd_create failed");
    }
}

void submit_job(job_scheduler *scheduler, job_item *job)
{
    enqueue_job(&scheduler->queues[glb_queue_id], job);
}

void wait_for_job(job_scheduler *scheduler, job_item *job)
{
    while (atomic_load_explicit(&job->open_work_items, memory_order_relaxed) > 0) {
        do_work(scheduler);
    }
}

void sync_threads(job_scheduler *scheduler)
{
    // scheduler->sync_threads = true

    // for (uint32_t i = 0; i < scheduler->num_threads + 1; i++)
    // 	reset_job_queue(&scheduler->queues[i]);
}

void join_threads(job_scheduler *scheduler)
{
    atomic_store_explicit(&scheduler->join_workers, true, memory_order_relaxed);

    scheduler->join_workers = true;

    for (uint32_t i = 0; i < scheduler->num_threads; i++) {
        LOG_CHECK(thrd_join(scheduler->threads[i], NULL), "thrd_join failed");
    }
}

static int32_t worker_thread(void *ptr)
{
    struct thrd_start_args *args = ptr;

    glb_queue_id = args->queue_id;
    set_thrd_id(args->queue_id);

    while (!atomic_load_explicit(&args->scheduler->join_workers, memory_order_relaxed)) {
        // check working signal?

        do_work(args->scheduler);
    }

    return 0;
}

static void do_work(job_scheduler *scheduler)
{
    job_item *work = find_work(scheduler);

    if (work != NULL) {
        if (work->job_fn != NULL) {
            work->job_fn(scheduler, work);
        }

        for (uint8_t i = 0; i < work->num_continuations; i++) {
            submit_job(scheduler, work->continuations[i]);
        }

        if (work->parent != NULL) {
            atomic_fetch_sub_explicit(&work->parent->open_work_items, 1, memory_order_relaxed);
        }

        atomic_fetch_sub_explicit(&work->open_work_items, 1, memory_order_relaxed);
    } else {
        thrd_yield();
    }
}

static job_item *find_work(job_scheduler *scheduler)
{
    job_item *work = dequeue_job(&scheduler->queues[glb_queue_id]);

    if (work != NULL) {
        return work;
    }

    uint32_t victim_queue = rand() % (scheduler->num_threads + 1);

    if (victim_queue == glb_queue_id) {
        return NULL;
    }

    return steal_job(&scheduler->queues[victim_queue]);
}
