#include <stdlib.h>

#include "job.h"
#include "macros.h"
#include "mem.h"

job_batch *create_job_batch(uint32_t capacity)
{
    size_t batch_sz = sizeof(job_batch) + sizeof(job_item *) * capacity;

    job_batch *batch = mem_alloc(FRAME_MEM, batch_sz, 0, "job_batch");
    batch->jobs = (job_item **)((uint8_t*)batch + sizeof(job_batch));
    batch->capacity = capacity;
    batch->open_jobs = 0;
    batch->num_continuations = 0;

    return batch;
}

// not thread safe. Use only from thread that creates batch
void add_job(job_batch *batch, uint64_t id, job_routine_fn job_fn, void *data)
{
    ASSERT(batch, "batch is null");
    ASSERT(job_fn, "job_fn is null");
    ASSERT(batch->open_jobs < batch->capacity, "job overflow");

    job_item *job = mem_alloc(FRAME_MEM, sizeof(job_item), 0, "job_item");
    job->batch = batch;
    job->id = id;
    job->job_fn = job_fn;
    job->data = data;

    batch->jobs[batch->open_jobs] = job;
    batch->open_jobs++;
}

// not thread safe. Use only from thread that creates batch
void add_continuation(job_batch *batch, job_batch *continuation)
{
    ASSERT(batch, "batch is null");
    ASSERT(continuation, "continuation is null");
    ASSERT(batch->num_continuations < MAX_CONTINUATIONS, "continuation overflow");

    batch->continuations[batch->num_continuations] = continuation;
    batch->num_continuations++;
}
