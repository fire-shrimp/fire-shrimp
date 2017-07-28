#include <stdint.h>
#include <string.h>

#include "error.h"
#include "job-queue.h"
#include "job.h"
#include "mem.h"

void init_job_queue(job_queue *queue, uint32_t capacity)
{
    ASSERT(capacity % 2 == 0, "invalid capacity");

    queue->jobs = mem_alloc(STATIC_MEM, sizeof(job_item *) * capacity, 0, "job_queue");
    queue->capacity = capacity;
    queue->bottom = 0u;
    queue->top = 0u;

    CHECK(mtx_init(&queue->mutex, mtx_plain), "mtx_init failed");
}

void reset_job_queue(job_queue *queue)
{
    queue->bottom = 0u;
    queue->top = 0u;
}

void enqueue_job(job_queue *queue, job_item *job)
{
    CHECK(mtx_lock(&queue->mutex), "mtx_lock failed");

    uint32_t i = queue->bottom & (queue->capacity - 1);

    ASSERT(i < queue->capacity, "job queue overflow %d %d %d", queue->bottom, queue->top, i);

    queue->jobs[i] = job;
    queue->bottom++;

    CHECK(mtx_unlock(&queue->mutex), "mtx_unlock failed");
}

job_item *dequeue_job(job_queue *queue)
{
    CHECK(mtx_lock(&queue->mutex), "mtx_lock failed");

    uint64_t num_jobs = queue->bottom - queue->top;
    job_item *job = NULL;

    if (num_jobs > 0) {
        queue->bottom--;
        uint32_t i = queue->bottom & (queue->capacity - 1);
        ASSERT(i < queue->capacity, "job queue overflow %d %d %d", queue->bottom, queue->top, i);
        job = queue->jobs[i];
    }

    CHECK(mtx_unlock(&queue->mutex), "mtx_unlock failed");

    return job;
}

job_item *steal_job(job_queue *queue)
{
    CHECK(mtx_lock(&queue->mutex), "mtx_lock failed");

    uint64_t num_jobs = queue->bottom - queue->top;
    job_item *job = NULL;

    if (num_jobs > 0) {
        uint32_t i = queue->top & (queue->capacity - 1);
        ASSERT(i < queue->capacity, "job queue overflow %d %d %d", queue->bottom, queue->top, i);
        job = queue->jobs[i];
        queue->top++;
    }

    CHECK(mtx_unlock(&queue->mutex), "mtx_unlock failed");

    return job;
}
