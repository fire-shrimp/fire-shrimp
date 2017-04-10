

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "char-buf.h"

struct game_template
{
};

struct entity_template
{

};

struct property_template
{

};

struct template_set
{
	struct game_template* game_templates;
	uint32_t num_game_templates;
	struct property_template* property_templates;
	uint32_t num_property_templates;
	struct entity_template* entity_templates;
	uint32_t num_entity_templates;
};

bool load_template_set_from_directory(struct template_set *templates, struct char_buf *path);

bool validate_template_set(struct template_set *templates);

bool free_template_set(struct template_set *templates);

