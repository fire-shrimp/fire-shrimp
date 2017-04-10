
#include "template.h"
#include "file-system.h"
#include "file.h"
#include "mem.h"
#include "char-buf.h"
#include "logger.h"
#include "macros.h"

static void parse_game_template_file(char *buf, struct char_buf *path, struct game_template *template, struct file_stats *file);
static void parse_property_template_file(char *buf, struct char_buf *path, struct property_template *template, struct file_stats *file);
static void parse_entity_template_file(char *buf, struct char_buf *path, struct entity_template *template, struct file_stats *file);

bool load_template_set_from_directory(struct template_set *templates, struct char_buf *path)
{
	struct directory_stats dir = {};

	bool success = open_directory(&dir, path);

	LOG_CHECK(success, "failed to open directory \"%s\" while loading templates", path->buf);

	templates->num_game_templates = 0;
	templates->num_property_templates = 0;
	templates->num_entity_templates = 0;

	for(int32_t i = 0; i < dir.num_files; i++)
	{
		if(compare_char_buf(dir.files[i].extension, "game"))
			templates->num_game_templates++;

		if(compare_char_buf(dir.files[i].extension, "property"))
			templates->num_property_templates++;

		if(compare_char_buf(dir.files[i].extension, "entity"))
			templates->num_entity_templates++;
	}

	templates->game_templates = MALLOC_HEAP(sizeof(struct game_template) * templates->num_game_templates, 0, "game templates");
	templates->property_templates = MALLOC_HEAP(sizeof(struct property_template) * templates->num_property_templates, 0, "property templates");
	templates->entity_templates = MALLOC_HEAP(sizeof(struct entity_template) * templates->num_entity_templates, 0,"entity templates");

	uint32_t current_game_template = 0;
	uint32_t current_property_template = 0;
	uint32_t current_entity_template = 0;

	char *buf = MALLOC_HEAP(MAX_TEMPLATE_FILE_SIZE, 0, "template file buffer");

	for(int32_t i = 0; i < dir.num_files; i++)
	{
		if(compare_char_buf(dir.files[i].extension, "game"))
			parse_game_template_file(buf, path, &templates->game_templates[current_game_template++], &dir.files[i]);

		if(compare_char_buf(dir.files[i].extension, "property"))
			parse_property_template_file(buf, path, &templates->property_templates[current_property_template++], &dir.files[i]);

		if(compare_char_buf(dir.files[i].extension, "entity"))
			parse_entity_template_file(buf, path, &templates->entity_templates[current_entity_template++], &dir.files[i]);
	}

	MEM_FREE_HEAP(buf);

	close_directory(&dir);

	return true;
}

bool free_template_set(struct template_set *templates)
{
	MEM_FREE_HEAP(templates->game_templates);
	MEM_FREE_HEAP(templates->property_templates);
	MEM_FREE_HEAP(templates->entity_templates);
}

static void parse_game_template_file(char *buf, struct char_buf *path, struct game_template *template, struct file_stats *file)
{
	char file_path_buf[path->size + MAX_FILE_NAME + 1];
	write_char_buf_format(file_path_buf, path->size + MAX_FILE_NAME + 1, path->size + MAX_FILE_NAME, "%s\\%s", path->buf, file->name);

	bool success = load_file(file_path_buf, buf, MAX_TEMPLATE_FILE_SIZE);

	LOG_CHECK(success, "failed to load game template \"%s\"", file->name);


}

static void parse_property_template_file(char *buf, struct char_buf *path, struct property_template *template, struct file_stats *file)
{

}

static void parse_entity_template_file(char *buf, struct char_buf *path, struct entity_template *template, struct file_stats *file)
{

}
