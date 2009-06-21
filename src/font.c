#include <stdlib.h>
#include <stdio.h>
#include "font.h"
#include "texture.h"

static struct list *
load_glyphs(const char *name)
{
	struct glyph *g;
	struct list *glyphs;
	char path[256];
	int ch;

	glyphs = list_new();
	
	printf("name: %s\n", name);

	for (ch = '0'; ch <= '9'; ch++) {
		g = malloc(sizeof *g);
		g->character = ch;
		snprintf(path, sizeof path, "fonts/%s/%c.png", name, ch);
		g->texture = load_texture_from_png(path, &g->width, &g->height);
		list_add(glyphs, (void *) g);
	}

	for (ch = 'a'; ch <= 'z'; ch++) {
		g = malloc(sizeof *g);
		g->character = ch;
		snprintf(path, sizeof path, "fonts/%s/%c.png", name, ch);
		g->texture = load_texture_from_png(path, &g->width, &g->height);
		list_add(glyphs, (void *) g);
	}

	for (ch = 'A'; ch <= 'Z'; ch++) {
		g = malloc(sizeof *g);
		g->character = ch;
		snprintf(path, sizeof path, "fonts/%s/%c.png", name, ch);
		g->texture = load_texture_from_png(path, &g->width, &g->height);
		list_add(glyphs, (void *) g);
	}



	return glyphs;
}

static void
glyph_destroy(struct glyph *g)
{
	glDeleteTextures(1, &g->texture);

	free(g);
}

struct font *
font_new(const char *name)
{
	struct font *f;

	f = malloc(sizeof *f);
	
	f->glyphs = load_glyphs(name);
	f->spacing = 10;

	return f;
}

struct glyph *
font_get_glyph(struct font *f, char ch)
{
	struct node *n;

	for (n = f->glyphs->first; n; n = n->next) {
		struct glyph *g = (struct glyph *) n->data;

		if (g->character == ch)
			return g;
	}

	return NULL;
}

void font_destroy(struct font *f)
{
	list_free(f->glyphs, glyph_destroy);

	free(f);
}
