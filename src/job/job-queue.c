#include <stdint.h>

#include "job-queue.h"
#include "job.h"
#include "macros.h"
#include "mem.h"

void init_job_queue(job_queue *queue, uint32_t max_jobs)
{
    queue->jobs = MALLOC_STATIC(sizeof(job_item *) * max_jobs, 0, "Job queue jobs");
    queue->max_jobs = max_jobs;
    queue->bottom = 0;
    queue->top = 0;

    LOG_CHECK(mtx_init(&queue->mutex, mtx_plain), "mtx_init failed");
}

void reset_job_queue(job_queue *queue)
{
    queue->bottom = 0;
    queue->top = 0;
}

void enqueue_job(job_queue *queue, job_item *job)
{
    LOG_CHECK(mtx_lock(&queue->mutex), "mtx_lock failed");

    LOG_ASSERT((queue->bottom - queue->top) < queue->max_jobs, "Job queue overflow");

    queue->jobs[queue->bottom] = job;
    queue->bottom++;

    LOG_CHECK(mtx_unlock(&queue->mutex), "mtx_unlock failed");
}

job_item *dequeue_job(job_queue *queue)
{
    LOG_CHECK(mtx_lock(&queue->mutex), "mtx_lock failed");

    job_item *job = NULL;
    const uint32_t num_jobs = queue->bottom - queue->top;

    if (num_jobs > 0) {
        queue->bottom--;
        job = queue->jobs[queue->bottom];
    }

    LOG_CHECK(mtx_unlock(&queue->mutex), "mtx_unlock failed");

    return job;
}

job_item *steal_job(job_queue *queue)
{
    LOG_CHECK(mtx_lock(&queue->mutex), "mtx_lock failed");

    job_item *job = NULL;
    const uint32_t num_jobs = queue->bottom - queue->top;

    if (num_jobs > 0) {
        job = queue->jobs[queue->top];
        queue->top++;
    }

    LOG_CHECK(mtx_unlock(&queue->mutex), "mtx_unlock failed");

    return job;
}
