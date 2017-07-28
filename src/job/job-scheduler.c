#include <stdbool.h>
#include <stdlib.h>

#include "error.h"
#include "job-scheduler.h"
#include "job.h"
#include "mem.h"
#include "thread-id.h"

typedef struct thrd_start_args {
    job_scheduler *scheduler;
    uint32_t queue_id;
} thrd_start_args;

_Thread_local uint32_t glb_queue_id;

static job_item *find_work(job_scheduler *scheduler)
{
    ASSERT(scheduler, "scheduler is null");

    job_item *work = dequeue_job(&scheduler->queues[glb_queue_id]);

    if (work != NULL) {
        return work;
    }

    uint32_t victim_queue = rand() % (scheduler->num_threads + 1);

    ASSERT(victim_queue < scheduler->num_threads + 1, "invalid victim queue %d",victim_queue);

    if (victim_queue == glb_queue_id) {
        return NULL;
    }

    return steal_job(&scheduler->queues[victim_queue]);
}

static void close_batch(job_scheduler *scheduler, job_batch *batch)
{
    if (atomic_fetch_sub_explicit(&batch->open_batches, 1, memory_order_seq_cst) == 0) {
        if(batch->parent != NULL){
            close_batch(scheduler, batch);
        }

        if(batch->continuation){
            submit_batch(scheduler, batch->continuation);
        }
    }
}

static void do_work(job_scheduler *scheduler)
{
    ASSERT(scheduler, "scheduler is null");

    job_item *work = find_work(scheduler);

    if (work != NULL) {
        if (work->job_fn != NULL) {
            work->job_fn(scheduler, work);
        }

        if (atomic_fetch_sub_explicit(&work->batch->open_jobs, 1, memory_order_seq_cst) == 1) {
            close_batch(scheduler, work->batch);
        }
    } else {
        thrd_yield();
    }
}

static int32_t worker_routine(void *ptr)
{
    ASSERT(ptr, "ptr is null");

    thrd_start_args *args = ptr;

    glb_queue_id = args->queue_id;
    set_thrd_id(args->queue_id);

    while (!atomic_load_explicit(&args->scheduler->join_workers, memory_order_seq_cst)) {
        do_work(args->scheduler);
    }

    return 0;
}

void init_scheduler(job_scheduler *scheduler, uint32_t num_threads)
{
    ASSERT(num_threads < MAX_WORKER_THREADS, "num_threads >= MAX_WORKER_THREADS");

    scheduler->num_threads = num_threads;
    scheduler->args = mem_alloc(STATIC_MEM, sizeof(thrd_start_args) * num_threads, 0, "job_scheduler");
    scheduler->join_workers = false;

    for (uint32_t i = 0; i < num_threads + 1; i++) {
        init_job_queue(&scheduler->queues[i], MAX_JOBS_PER_QUEUE);
    }

    // main thread uses first queue
    glb_queue_id = 0;

    for (uint32_t i = 0; i < num_threads; i++) {
        scheduler->args[i].scheduler = scheduler;
        scheduler->args[i].queue_id = i + 1;

        uint32_t result = thrd_create(&scheduler->threads[i], worker_routine, (void *)&scheduler->args[i]);
        CHECK(result, "thrd_create failed");
    }
}

void submit_batch(job_scheduler *scheduler, job_batch *batch)
{
    ASSERT(scheduler, "scheduler is null");
    ASSERT(batch, "batch is null");

    for(uint32_t i = 0; i < batch->capacity; i++){
        enqueue_job(&scheduler->queues[glb_queue_id], batch->jobs[i]);
    }
}

void wait_for_batch(job_scheduler *scheduler, job_batch *batch)
{
    ASSERT(scheduler, "scheduler is null");
    ASSERT(batch, "batch is null");

    while (atomic_load_explicit(&batch->open_batches, memory_order_seq_cst) > 0) {
        do_work(scheduler);
    }
}

void join_threads(job_scheduler *scheduler)
{
    ASSERT(scheduler, "scheduler is null");

    atomic_store_explicit(&scheduler->join_workers, true, memory_order_seq_cst);

    scheduler->join_workers = true;

    for (uint32_t i = 0; i < scheduler->num_threads; i++) {
        uint32_t result = thrd_join(scheduler->threads[i], NULL);
        CHECK(result, "thrd_join failed");
    }
}
