#include <stdlib.h>

#include "job.h"
#include "log.h"
#include "macros.h"
#include "mem.h"

job_item *create_job(uint64_t id, job_routine_fn job_fn, job_item *parent, void *data)
{
    job_item *job = MALLOC_FRAME(sizeof(job_item), 0, "job create");
    job->id = id;
    job->job_fn = job_fn;
    job->parent = parent;
    job->data = data;
    job->open_work_items = 1;
    job->num_continuations = 0;

    if (parent != NULL) {
        atomic_fetch_add_explicit(&parent->open_work_items, 1, memory_order_relaxed);
    }

    return job;
}

void add_continuation(job_item *job, job_item *dependency)
{
    LOG_ASSERT(job, "job is null");
    LOG_ASSERT(dependency, "dependency is null");

    if (job->num_continuations >= MAX_CONTINUATIONS) {
        LOG_ERROR(CORE_LOG, "Adding continuation to job %d failed", job->id);
        return;
    }

    // not thread safe. Use only from thread that creates antecedant
    job->continuations[job->num_continuations] = dependency;
    job->num_continuations++;
}
