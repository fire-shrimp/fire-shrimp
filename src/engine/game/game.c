
#include "game.h"
#include "template.h"

void load_game(struct game_data *game,  struct char_buf *path)
{
	struct template_set templates = {};
	load_template_set_from_directory(&templates, path);

	free_template_set(&templates);

	return;
}
