

#include <stdlib.h>

#include "job.h"
#include "mem.h"
#include "logger.h"
#include "macros.h"

struct job_item* job_create(uint64_t id, job_routine_fn job_fn, struct job_item *parent, void *data)
{
	struct job_item *job = MALLOC_FRAME(sizeof(struct job_item), 0, "job create");
	job->id = id;
	job->job_fn = job_fn;
	job->parent = parent;
	job->data = data;
	job->open_work_items = 1;
	job->num_continuations = 0;

	if(parent != NULL)
		atomic_fetch_add_explicit(&parent->open_work_items, 1, memory_order_relaxed);

	return job;
}

void job_add_continuation(struct job_item *job, struct job_item *dependency)
{
	LOG_ASSERT(job, "job is null");
	LOG_ASSERT(dependency, "dependency is null");

	if(job->num_continuations >= MAX_CONTINUATIONS)
	{
		LOG_ERROR(CORE_LOG, "Adding continuation to job %d failed", job->id);
		return;
	}

	//not thread safe. Use only from thread that creates antecedant
	job->continuations[job->num_continuations] = dependency;
	job->num_continuations++;
}
