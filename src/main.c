/*
 * main.c - Main game logic
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <time.h>

#include "panic.h"
#include "util.h"
#include "defs.h"
#include "gameplay.h"

/*
 * CONSTANTS AND ENUMERATIONS
 */

enum State
{
	GAMEPLAY,
	MAIN_MENU
};

static const char *WINDOW_CAPTION = "Shitty game";

static void
initialize_sdl(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		panic("SDL_Init: %s", SDL_GetError());

	if (!SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, /* SDL_FULLSCREEN | */SDL_OPENGL))
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
initialize_opengl(void)
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
check_gl_errors(void)
{
	GLenum error;

	if ((error = glGetError()) != GL_NO_ERROR)
		panic("OpenGL error: %s\n", gluErrorString(error));
}

static void
tear_down_sdl(void)
{
	SDL_Quit();
}

static void
tear_down(void)
{
	tear_down_gameplay();
	tear_down_sdl();
}

static void
initialize(int argc, char *argv[])
{
	srand(time(NULL));
	initialize_sdl();
	initialize_opengl();
	initialize_gameplay_data();
}

static void
handle_events(void)
{
	int running = 1;
	int status = GAMEPLAY;
	unsigned prev_ticks;

	while (running) {
		SDL_Event event;
		int delay;

		prev_ticks = SDL_GetTicks();

		while (SDL_PollEvent(&event)) {
			if (status == GAMEPLAY) {
				int rv = handle_gameplay_events(event);

				if (rv == 1)
					running = 0;
			}
		}


		if (status == GAMEPLAY)
			handle_gameplay_updates();

		delay = 1000/FPS - (SDL_GetTicks() - prev_ticks);

		check_gl_errors();

		if (delay > 0)
			SDL_Delay(delay);
	}
}


int
main(int argc, char *argv[])
{
	initialize(argc, argv);
	handle_events();
	tear_down();

	return 0;
}
