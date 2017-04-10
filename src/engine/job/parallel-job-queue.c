

#include <stdint.h>

#include "parallel-job-queue.h"
#include "job.h"
#include "mem.h"
#include "macros.h"

void job_queue_init(struct parallel_job_queue *queue, uint32_t max_jobs)
{
	queue->jobs =  MALLOC_STATIC(sizeof(struct job_item*) * max_jobs, 0, "Job queue jobs");
	queue->max_jobs = max_jobs;
	queue->bottom = 0;
	queue->top = 0;

	LOG_CHECK(mtx_init(&queue->mutex, mtx_plain), "mtx_init failed");
}

void job_queue_reset(struct parallel_job_queue *queue)
{
	queue->bottom = 0;
	queue->top = 0;
}

void job_queue_push(struct parallel_job_queue *queue, struct job_item *job)
{
	LOG_CHECK(mtx_lock(&queue->mutex), "mtx_lock failed");

	LOG_ASSERT((queue->bottom - queue->top) < queue->max_jobs, "Job queue overflow");

	queue->jobs[queue->bottom] = job;
	queue->bottom++;

	LOG_CHECK(mtx_unlock(&queue->mutex), "mtx_unlock failed");
}

struct job_item* job_queue_pop(struct parallel_job_queue *queue)
{
	LOG_CHECK(mtx_lock(&queue->mutex), "mtx_lock failed");

	struct job_item *job = NULL;
	const uint32_t num_jobs = queue->bottom - queue->top;

	if (num_jobs > 0)
	{
		queue->bottom--;
		job = queue->jobs[queue->bottom];
	}

	LOG_CHECK(mtx_unlock(&queue->mutex), "mtx_unlock failed");

	return job;
}

struct job_item* job_queue_steal(struct parallel_job_queue *queue)
{
	LOG_CHECK(mtx_lock(&queue->mutex), "mtx_lock failed");

	struct job_item *job = NULL;
	const uint32_t num_jobs = queue->bottom - queue->top;

	if (num_jobs > 0)
	{
		job = queue->jobs[queue->top];
		queue->top++;
	}

	LOG_CHECK(mtx_unlock(&queue->mutex), "mtx_unlock failed");

	return job;
}
