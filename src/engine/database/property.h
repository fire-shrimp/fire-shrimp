#pragma once

#include <stdint.h>

#include "property-schema.h"
#include "associative-array.h"

struct property
{
	void *data;
	uint32_t capacity, count;
	size_t stride;
	struct associative_array index;
	struct schema *schema;
};
