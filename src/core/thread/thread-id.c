

#include "thread-id.h"

_Thread_local uint32_t glb_thrd_id = 0;

void set_thrd_id(uint32_t thrd_id)
{
	glb_thrd_id = thrd_id;
}

uint32_t get_thrd_id()
{
	return glb_thrd_id;
}

