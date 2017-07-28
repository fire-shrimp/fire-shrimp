#include <stdlib.h>

#include "error.h"
#include "job.h"
#include "mem.h"

//only safe to add child batch if :
//
// a)   parent batch has not begun executing
// b)   child added to parent in same executing heirarchy
//      ie from job that belongs to parent batch
job_batch *create_job_batch(uint32_t capacity, job_batch *parent)
{
    size_t batch_sz = sizeof(job_batch) + sizeof(job_item *) * capacity;

    job_batch *batch = mem_alloc(FRAME_MEM, batch_sz, 0, "job_batch");
    batch->jobs = (job_item **)((uint8_t *)batch + sizeof(job_batch));
    batch->capacity = capacity;
    batch->open_jobs = 0;
    batch->open_batches = 1;
    batch->parent = NULL;
    batch->continuation = NULL;

    if(parent != NULL){
        batch->parent = parent;
        atomic_fetch_add_explicit(&parent->open_batches, 1, memory_order_seq_cst);
    }

    return batch;
}

// not thread safe. Use only from thread that creates batch, and only BEFORE submitting batch
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

    batch->continuation = continuation;
}
