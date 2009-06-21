#ifndef MENU_H
#define MENU_H

#include "list.h"
#include "font.h"

typedef void (*menu_action_cb)(void);

struct menu
{
	struct list *items;
	struct font *font;

	int x;
	int y;
};

struct menu *
menu_new(int x, int y, struct font *font);

void menu_destroy(struct menu *m);
void menu_add_item(struct menu *m, const char *caption, 
		menu_action_cb action);
void menu_draw(const struct menu *menu);
void menu_highlight_next(struct menu *menu);
void menu_highlight_prev(struct menu *menu);

#endif /* MENU_H */
