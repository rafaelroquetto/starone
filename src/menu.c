#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "menu.h"

enum {
	MENU_ITEM_SPACING = 50,
	TEXT_SPACING_FACTOR = 3
};

struct color
{
	float r, g, b;
};

struct menu_item
{
	char *caption;

	menu_action_cb action;

	int highlighted;
};


static struct color color_white = { 1.0, 1.0, 1.0 };
static struct color color_yellow = { 1.0, 1.0, 0.0 };

static void 
draw_text(const char *text, float xi, float yi,
	       	struct color *color, struct font *font)
{
	const struct glyph *g;
	float x, y;
	int spacing;

	x = 0;
	y = 0;

	glColor3f(color->r, color->g, color->b);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(xi, yi, 0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	spacing = font->spacing;

	while (*text) {
		if (*text == ' ') {
			x += TEXT_SPACING_FACTOR*spacing;

			text++;
		} else {

			g = font_get_glyph(font, *text);

			if (g == NULL)
				panic("Cannot load glyph for '%c'.", *text);

			glBindTexture(GL_TEXTURE_2D, g->texture);

			glBegin(GL_QUADS);

			glTexCoord2f(0.0, 0.0);
			glVertex2f(x, y);

			glTexCoord2f(1.0, 0.0);
			glVertex2f(x + g->width, y);

			glTexCoord2f(1.0, 1.0);
			glVertex2f(x + g->width, g->height);

			glTexCoord2f(0.0, 1.0);
			glVertex2f(x, g->height);

			x += g->width + spacing;

			glEnd();

			text++;
		}
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	glPopMatrix();
}


static struct menu_item *
menu_item_new(const char *caption, menu_action_cb cb)
{
	struct menu_item *i;

	i = malloc(sizeof *i);

	i->caption = strdup(caption);
	i->action = cb;
	i->highlighted = 0;

	return i;
}

static void
menu_item_destroy(struct menu_item *i)
{
	free(i->caption);
	free(i);
}

struct menu *
menu_new(int x, int y, struct font *font)
{
	struct menu *m;

	assert(font);

	m = malloc(sizeof *m);

	m->items = list_new();
	m->x = x;
	m->y = y;
	m->font = font;

	return m;
}

void menu_destroy(struct menu *m)
{
	list_free(m->items, menu_item_destroy);
	
	free(m);
}

void menu_add_item(struct menu *m, const char *caption, 
		menu_action_cb action)
{
	struct menu_item *item;

	item = menu_item_new(caption, action);

	if (list_size(m->items) == 0) {
		item->highlighted = 1;
	}

	list_add(m->items, (void *) item);
}

void menu_draw(const struct menu *menu)
{
	struct node *current;
	struct color *color;
	int x, y;

	x = menu->x;
	y = menu->y;

	for (current = menu->items->first; current; current = current->next) {
		struct menu_item *item = (struct menu_item *) current->data;

		color = (item->highlighted) ? &color_yellow : &color_white;

		draw_text(item->caption, x, y, color, menu->font);

		y += MENU_ITEM_SPACING;
	}
}

void menu_highlight_next(struct menu *menu)
{
	struct node *c;

	for (c = menu->items->first; c; c = c->next) {
		struct menu_item *next;
		struct menu_item *item = (struct menu_item *) c->data;

		if (item->highlighted) {
			item->highlighted = 0;

			if (c->next != NULL) {
				next = (struct menu_item *) c->next->data;
			} else {
				next = (struct menu_item *) menu->items->first->data;
			}

			next->highlighted = 1;

			break;
		}
	}
}

void menu_highlight_prev(struct menu *menu)
{
	struct node *c;
	struct node *last;

	for (c = menu->items->first; c; c = c->next) {
		struct menu_item *prev;
		struct menu_item *item = (struct menu_item *) c->data;

		if (item->highlighted) {
			item->highlighted = 0;

			if (c->prev != NULL) {
				prev = (struct menu_item *) c->prev->data;
			} else {
				for (last = menu->items->first; last->next; last = last->next) ;
				prev = (struct menu_item *) last->data;
			}

			prev->highlighted = 1;

			break;
		}
	}
}

void menu_activate_item(const struct menu *menu)
{
	struct node *c;

	for (c = menu->items->first; c; c = c->next) {
		struct menu_item *item = (struct menu_item *) c->data;

		if (item->highlighted && item->action) {
			item->action();

			break;
		}
	}
}
