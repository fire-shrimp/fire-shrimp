#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "char-buf.h"

typedef struct game_template {
} game_template;

typedef struct entity_template {
} entity_template;

typedef struct property_template {
} property_template;

typedef struct template_set {
    game_template *game_templates;
    uint32_t num_game_templates;
    property_template *property_templates;
    uint32_t num_property_templates;
    entity_template *entity_templates;
    uint32_t num_entity_templates;
} template_set;

bool load_template(template_set *templates, char_buf *path);

bool validate_template(template_set *templates);

bool free_template(template_set *templates);
