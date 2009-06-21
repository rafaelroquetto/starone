/*
 * mainmenu.c - Main menu container
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */

#include <GL/gl.h>
#include "mainmenu.h"

#include "font.h"
#include "label.h" #include "menu.h" 
enum {
	QUIT = 1,
	NEWGAME = 2
};

static struct label *title_label;
static struct font *font;
static struct menu *menu;

/* context variables */
static int quit;
static int newgame;
static int redraw_menu;

static void
draw_menu(void)
{
	label_draw(title_label);
	menu_draw(menu);
}

static void
redraw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	draw_menu();
	SDL_GL_SwapBuffers();
}

static void
quit_callback(void)
{
	quit = 1;
}

static void
newgame_callback(void)
{
	newgame = 1;
}

static void
settings_callback(void)
{
	fprintf(stderr, "settings_callback(): fixme!\n");
}

void handle_mainmenu_updates(void)
{
	/* SCHEISSE! */
	static int first_run = 1;

	if (redraw_menu || first_run) {
		first_run = 0;
		redraw();
	}
}

void tear_down_mainmenu(void)
{
	label_destroy(title_label);
	menu_destroy(menu);
	font_destroy(font);
}

void initialize_mainmenu_data(void)
{
	quit = 0;
	newgame = 0;

	font = font_new("ngage");

	title_label = label_new("Star One", 100, 50, font);

	menu = menu_new(30, 150, font);

	menu_add_item(menu, "Quit", quit_callback);
	menu_add_item(menu, "Settings", settings_callback);
	menu_add_item(menu, "New Game", newgame_callback);
}

int handle_mainmenu_events(SDL_Event event)
{
	if (quit)
		return QUIT;

	if (newgame)
		return NEWGAME;

	redraw_menu = 1;

	switch (event.type) {
		case SDL_QUIT:
			return QUIT;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				return QUIT;
			} else if (event.key.keysym.sym == SDLK_DOWN) {
				menu_highlight_next(menu);
			} else if (event.key.keysym.sym == SDLK_UP) {
				menu_highlight_prev(menu);
			} else if (event.key.keysym.sym == SDLK_KP_ENTER) {
				menu_activate_item(menu);
			} else if (event.key.keysym.sym == SDLK_RETURN) {
				menu_activate_item(menu);
			} else {
				redraw_menu = 0;
			}
			break;
		
		default:
			redraw_menu = 0;
	}

	return 0;
}
