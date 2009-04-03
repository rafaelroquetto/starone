#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <SDL.h>
#include <GL/glew.h>
#include <math.h>

#include "panic.h"
#include "ship.h"
#include "beam.h"
#include "list.h"
#include "util.h"
#include "defs.h"

enum {
	PAD_UP = 1,
	PAD_DOWN = 2,
	PAD_LEFT = 4,
	PAD_RIGHT = 8,
	PAD_FIRE = 16,
};

static const char *WINDOW_CAPTION = "Shitty game";

static struct ship *enterprise = NULL;

static struct node *ship_list = NULL;

static unsigned pad_state;

static void
initialize_sdl()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		panic("SDL_Init: %s", SDL_GetError());

	if (!SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_OPENGL))
		panic("SDL_SetVideoMode: %s", SDL_GetError());

	SDL_WM_SetCaption(WINDOW_CAPTION, NULL);
}

static void
resize_viewport(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.f, width, height, 0.f, -1.f, 1.f);
	glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
}

static void
initialize_opengl()
{
	glClearColor(0, 0, 0, 0);

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);

	resize_viewport(WINDOW_WIDTH, WINDOW_HEIGHT);
}

static void
check_gl_errors()
{
	GLenum error;

	if ((error = glGetError()) != GL_NO_ERROR)
		panic("OpenGL error: %s\n", gluErrorString(error));
}

static void
tear_down_sdl()
{
	SDL_Quit();
}

static void
update_ships()
{
	struct node *current;
	struct ship *s;

	current = ship_list;

	while (current) {
		s = (struct ship *) current->data;
		ship_update(s);

		current = current->next;
	}
}

static void
draw_ships()
{
	struct node *c;

	if (ship_list == NULL)
		return;

	c = ship_list;

	while (c != NULL) {
		ship_draw((struct ship *) c->data);
		c = c->next;
	}
}

static void
do_test()
{
	draw_ships();
}

static void
redraw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	do_test();
	SDL_GL_SwapBuffers();
}

static struct ship *
create_ship(int x, int y)
{
	struct ship *s = ship_new(x, y);

	ship_list = list_push(ship_list,
			(void *) s);

	return s;
}

static void
initialize_data()
{
	enterprise = create_ship(50, 50);
}

static void
initialize(int argc, char *argv[])
{
	initialize_sdl();
	initialize_opengl();
	initialize_data();
}

static void
handle_events()
{
	int running = 1;
	unsigned prev_ticks;

	redraw();

	while (running) {
		SDL_Event event;
		unsigned delay;

		prev_ticks = SDL_GetTicks();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = 0;
					break;

				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_UP) {
						pad_state |= PAD_UP;
					} else if (event.key.keysym.sym == SDLK_DOWN) {
						pad_state |= PAD_DOWN;
					} else if (event.key.keysym.sym == SDLK_LEFT) {
						pad_state |= PAD_LEFT;
					} else if (event.key.keysym.sym == SDLK_RIGHT) {
						pad_state |= PAD_RIGHT;
					} else if (event.key.keysym.sym == SDLK_SPACE) {
							pad_state |= PAD_FIRE;
					}
					break;

				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_UP) {
						pad_state &= ~PAD_UP;
					} else if (event.key.keysym.sym == SDLK_DOWN) {
						pad_state &= ~PAD_DOWN;
					} else if (event.key.keysym.sym == SDLK_LEFT) {
						pad_state &= ~PAD_LEFT;
					} else if (event.key.keysym.sym == SDLK_RIGHT) {
						pad_state &= ~PAD_RIGHT;
					} else if (event.key.keysym.sym == SDLK_SPACE) {
						pad_state &= ~PAD_FIRE;
					}
					break;

				default:
					break;
			}
		}

		if (pad_state & PAD_UP)
			ship_move_forward(enterprise);
		if (pad_state & PAD_DOWN)
			ship_move_backwards(enterprise);
		if (pad_state & PAD_LEFT)
			ship_rotate_countercw(enterprise);
		if (pad_state & PAD_RIGHT)
			ship_rotate_cw(enterprise);
		if (pad_state & PAD_FIRE) {
			if (ship_can_fire(enterprise)) {
				ship_fire_front(enterprise);
			}
		}

		redraw();
		update_ships();

		delay = 1000/FPS - (SDL_GetTicks() - prev_ticks);

		if (delay > 0)
			SDL_Delay(delay);
	}
}

static void
tear_down()
{
	tear_down_sdl();
}

int
main(int argc, char *argv[])
{
	initialize(argc, argv);
	handle_events();
	tear_down();

	return 0;
}
