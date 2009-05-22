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
#include <math.h>
#include <time.h>

#include "panic.h"
#include "asteroid.h"
#include "ship.h"
#include "beam.h"
#include "particle.h"
#include "explosion.h"
#include "list.h"
#include "util.h"
#include "defs.h"

enum Borders {
	TOP,
	RIGHT,
	BOTTOM,
	LEFT
};

enum {
	PAD_UP = 1,
	PAD_DOWN = 2,
	PAD_LEFT = 4,
	PAD_RIGHT = 8,
	PAD_FIRE = 16,
	PAD_PULSE = 32
};

enum { 	
	MAX_ASTEROIDS = 30,
	MIN_ASTEROIDS = 10,
};

static const char *WINDOW_CAPTION = "Shitty game";

static struct ship *enterprise = NULL;

static struct ship *enterprise2 = NULL;

static struct list *ship_list = NULL;

static struct list *asteroid_list = NULL;

static struct list *explosion_list = NULL;

static unsigned asteroid_obound_count = 0;

static unsigned pad_state;


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

static int
beam_hit_asteroid(const struct beam *b,
		const struct asteroid *a)
{
	float d_square;
	float r_square;
	float dx = (b->x - a->x);
	float dy = (b->y - a->y);

	d_square = dx*dx + dy*dy;

	r_square = a->radius;
	r_square *= r_square;

	return (d_square < r_square);
}

static int
asteroid_hit_asteroid(const struct asteroid *a,
		const struct asteroid *b)
{
	float d_square;
	float r_square;
	float dx = (b->x - a->x);
	float dy = (b->y - a->y);

	d_square = dx*dx + dy*dy;

	r_square = a->radius;
	r_square *= r_square;

	return (d_square < r_square);
}

static void
create_explosion(float x, float y)
{
	struct explosion *e;

	if (explosion_list == NULL) explosion_list = list_new();

	
	e = explosion_new(x, y);
	
	list_add(explosion_list, (void *) e);
}

static void
check_beam_collisions(void)
{
	struct asteroid *asteroid;
	struct beam *beam;

	struct node *b;
	struct node *a;
	struct list *beam_list = ship_get_beam_list(enterprise);

	for (b = beam_list->first; b; b = b->next) {

		beam = (struct beam *) b->data;

		for (a = asteroid_list->first; a; a = a->next) {

			asteroid = (struct asteroid *) a->data;

			if (beam_hit_asteroid(beam, asteroid)) {
				create_explosion(asteroid->x, asteroid->y);
				asteroid_remove(asteroid);
				beam_remove(beam);

			}
		}
	}
}

static void
check_asteroid_collisions(void)
{
	struct node *a;
	struct node *b;
	struct asteroid *current;
	struct asteroid *iterator;

	for (a = asteroid_list->first; a; a = a->next) {
		current = (struct asteroid *) a->data;

		for (b = asteroid_list->first; b; b = b->next) {
			iterator = (struct asteroid *) b->data;

			if (iterator == current)
				continue;

			if (asteroid_hit_asteroid(current, iterator)) {
				asteroid_collide(current, iterator);
			}
		}
	}
}

static void
check_collisions(void)
{
	check_beam_collisions();
	check_asteroid_collisions();
}


static void 
respawn_asteroids(void)
{
	int i, aux, type, create, n_asteroids;
	float x, y, direction, radius;
	struct asteroid *a;

	srand(time(NULL));

	type = rand() % 2;

	n_asteroids = list_size(asteroid_list);

	radius = asteroid_radius_by_type(type);
	
	if (n_asteroids == MAX_ASTEROIDS)
		return;

	create = (rand() % (MAX_ASTEROIDS - n_asteroids)) + asteroid_obound_count; 

	for (i = create; i > 0; i--) {
		aux = rand() % 4;

		if (aux == TOP) {
			x = rand() % WINDOW_WIDTH;
			y = 0 - 2*radius;
			direction = (rand() % 180);
		} else if (aux == RIGHT) {
			x = WINDOW_WIDTH + 2*radius;
			y = rand() % WINDOW_HEIGHT;
			direction = 90.0 + (rand() % 180);
		} else if (aux == BOTTOM) {
			x = rand() % WINDOW_WIDTH;
			y = WINDOW_HEIGHT + 2*radius;
			direction = 181.0 + (rand() % 180);
		} else if (aux == LEFT) {
			x = 0 - 2*radius;
			y = rand() % WINDOW_HEIGHT;
			direction = (270 + (rand() % 180)) % 360;
		} else {
			abort();
		}		
	
		a = asteroid_new(x, y, direction, type);
		
		list_add(asteroid_list, (void *) a);
		asteroid_obound_count--;
	}

}

static void
update_asteroids(void)
{

	struct node *current;
	struct asteroid *a;
	struct asteroid *spare;


	current = asteroid_list->first;

	while (current) {
		struct node *next = current->next;

		a = (struct asteroid *) current->data;
		asteroid_update(a);

		if (a->remove || asteroid_out_of_bounds(a,
			WINDOW_WIDTH, WINDOW_HEIGHT)) {

			spare = list_remove(asteroid_list, current);

			asteroid_destroy(spare);

			asteroid_obound_count++;

		}

		current = next;
	}

	respawn_asteroids();
}




static void
update_explosions(void)
{
	struct node *current;
	struct explosion *e;


	current = explosion_list->first;

	while (current) {
		struct node *next = current->next;

		e = (struct explosion *) current->data;
		explosion_update(e);

		if (explosion_done(e)) {
			list_remove(explosion_list, current);

			explosion_destroy(e);
		}

		current = next;
	}
}

static void
update_ships(void)
{
	struct node *current;
	struct ship *s;

	current = ship_list->first;

	while (current) {
		s = (struct ship *) current->data;
		ship_update(s);

		current = current->next;
	}
}

static void
draw_ships(void)
{
	struct node *c;

	if (ship_list == NULL)
		return;

	c = ship_list->first;

	while (c != NULL) {
		ship_draw((struct ship *) c->data);
		c = c->next;
	}
}

static void
draw_asteroids(void)
{
	struct node *c;

	if (asteroid_list == NULL)
		return;

	c = asteroid_list->first;

	while (c != NULL) {
		asteroid_draw((struct asteroid *) c->data);
		c = c->next;
	}
}

static void
draw_explosions(void)
{
	struct node *c;

	if (explosion_list == NULL)
		return;

	c = explosion_list->first;

	while (c != NULL) {
		explosion_draw((struct explosion *) c->data);
		c = c->next;
	}
}

static void
do_test(void)
{
	draw_ships();
	draw_asteroids();
	draw_explosions();
}

static void
redraw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	do_test();
	SDL_GL_SwapBuffers();
}

static struct ship *
create_ship(int x, int y)
{
	struct ship *s = ship_new(x, y);

	if (ship_list == NULL)
		ship_list = list_new();

	list_add(ship_list, (void *) s);

	return s;
}

static void
create_asteroids(void)
{
	int i, x, y, direction, type;
	struct asteroid *a;

	srand(time(NULL));

	if (asteroid_list == NULL)
		asteroid_list = list_new();


	for (i = 0; i < MIN_ASTEROIDS; i++) {
		x = rand() % WINDOW_WIDTH;
		y = rand() % WINDOW_HEIGHT;
		type = rand() % 2;
		direction = rand() % 360;

		a = asteroid_new(x, y, direction, type);

		list_add(asteroid_list, (void *) a);
	}
}


static void
initialize_data(void)
{
	enterprise = create_ship(50, 50);
	explosion_list = list_new();

	create_asteroids();
}

static void
initialize(int argc, char *argv[])
{
	srand(time(NULL));
	initialize_sdl();
	initialize_opengl();
	initialize_data();
}

static void
handle_events(void)
{
	int running = 1;
	unsigned prev_ticks;

	redraw();

	while (running) {
		SDL_Event event;
		int delay;

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
					} else if (event.key.keysym.sym == SDLK_m) {
							pad_state |= PAD_PULSE;
					} else if (event.key.keysym.sym == SDLK_ESCAPE) {
						running = 0;
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
					} else if (event.key.keysym.sym == SDLK_m) {
						pad_state &= ~PAD_PULSE;
					}
					break;

				default:
					break;
			}
		}

		if (pad_state & PAD_UP)
			ship_throttle(enterprise);
		if (pad_state & PAD_DOWN)
			ship_break_and_reverse(enterprise);
		if (pad_state & PAD_LEFT)
			ship_rotate_countercw(enterprise);
		if (pad_state & PAD_RIGHT)
			ship_rotate_cw(enterprise);
		if (pad_state & PAD_FIRE) {
			if (ship_can_fire(enterprise)) {
				ship_fire_front(enterprise);
			}
		}
		if (pad_state & PAD_PULSE) {
			if (ship_can_pulse(enterprise)) {
				ship_pulse(enterprise);
			}
		}

		redraw();
		check_collisions();
		update_ships();
		update_asteroids();
		update_explosions();

		delay = 1000/FPS - (SDL_GetTicks() - prev_ticks);

		check_gl_errors();

		if (delay > 0)
			SDL_Delay(delay);
	}
}


static void
free_asteroids(void)
{
	list_free(asteroid_list, asteroid_destroy);
}

static void
free_explosions(void)
{
	list_free(explosion_list, explosion_destroy);
}

static void
free_objects(void)
{
	free_asteroids();
	free_explosions();
	ship_destroy(enterprise);
}

static void
tear_down(void)
{
	free_objects();
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
