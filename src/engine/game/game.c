
#include "game.h"
#include "template.h"

void load_game(game_data *game, char_buf *path)
{
    template_set templates = {};
    load_template(&templates, path);

    free_template(&templates);

    return;
}
