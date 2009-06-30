/*
 * gameplay.c - Main game logic
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
#include "asteroid.h"
#include "ship.h"
#include "beam.h"
#include "particle.h"
#include "explosion.h"
#include "list.h"
#include "defs.h"
#include "collisions.h"
#include "label.h"
#include "font.h"

/*
 * CONSTANTS AND ENUMERATIONS
 */

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

static struct ship *enterprise = NULL;

static struct ship *enterprise2 = NULL;

static struct list *ship_list = NULL;

static struct list *asteroid_list = NULL;

static struct list *explosion_list = NULL;

static unsigned asteroid_obound_count = 0;

static struct label *score_label1 = NULL;

static struct label *score_label2 = NULL;

static struct font *font = NULL;

static unsigned pad_state;
static unsigned pad2_state;


static void
create_explosion(float x, float y)
{
	struct explosion *e;

	if (explosion_list == NULL) 
		explosion_list = list_new();

	
	e = explosion_new(x, y);
	
	list_add(explosion_list, (void *) e);
}

static void
asteroid_collision_callback(struct asteroid *a,
		struct beam *b)
{
	create_explosion(a->x, a->y);
	asteroid_remove(a);
	beam_remove(b);
}

static void
asteroid_ship_collision_callback(struct asteroid *a,
		struct ship *s)
{
	create_explosion(a->x, a->y);
	ship_respawn(s);
	asteroid_remove(a);
	
}

static void
asteroid_crystal_collision_callback(struct asteroid *a,
		struct ship *s)
{
	/* TODO: draw effects */
	asteroid_remove(a);
}

static void
check_collisions(void)
{
	check_beam_collisions(ship_list, asteroid_list,
			asteroid_collision_callback);

	check_asteroid_collisions(asteroid_list, ship_list, 
			asteroid_collide, 
			asteroid_ship_collision_callback,
			asteroid_crystal_collision_callback);

	check_ship_collisions(ship_list, ship_collide);
}


static void 
respawn_asteroids(void)
{
	int i, aux, type, create, n_asteroids;
	float x, y, direction, radius, speed;
	struct asteroid *a;

	srand(time(NULL));

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

		type = rand() % NUM_ASTERORID_TYPES;
		speed = 1;
		a = asteroid_new(x, y, direction, type, speed);
		
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
		
		if(ship_hit_bounds(s, WINDOW_WIDTH, WINDOW_HEIGHT))
			s->speed = -s->speed*1.5;

		current = current->next;
	}
}

static void
update_lives(void)
{
	int ship1_lives;
	int ship2_lives;
	char buf[16];

	ship1_lives = ship_get_lives(enterprise);
	ship2_lives = ship_get_lives(enterprise2);

	snprintf(buf, sizeof buf, "%d", ship1_lives);

	if (ship1_lives >= 0) {
		label_set_text(score_label1, buf);
	}

	snprintf(buf, sizeof buf, "%d", ship2_lives);

	if (ship2_lives >= 0) {
		label_set_text(score_label2, buf);
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
	
	label_draw(score_label1);
	label_draw(score_label2);
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
	float speed;
	struct asteroid *a;

	srand(time(NULL));

	if (asteroid_list == NULL)
		asteroid_list = list_new();


	for (i = 0; i < MIN_ASTEROIDS; i++) {
		x = rand() % WINDOW_WIDTH;
		y = rand() % WINDOW_HEIGHT;
		type = rand() % NUM_ASTERORID_TYPES;
		direction = rand() % 360;
		speed = (3 + (rand() % 201)) / 200.0;

		a = asteroid_new(x, y, direction, type, speed);

		list_add(asteroid_list, (void *) a);
	}
}


void initialize_gameplay_data(void)
{
	enterprise = create_ship(50, 50);
	enterprise2 = create_ship(200, 200);

	font = font_new("ngage");
	score_label1 = label_new("0", 10, 10, font);
	score_label2 = label_new("0", WINDOW_WIDTH - 100, 10, font);

	ship_set_color(enterprise2, 0.3, 1.0, 1.0);

	explosion_list = list_new();

	create_asteroids();
}

int handle_gameplay_events(SDL_Event event)
{
	redraw();

	switch (event.type) {
		case SDL_QUIT:
			return 1;
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
			} else if (event.key.keysym.sym == SDLK_w) {
				pad2_state |= PAD_UP;
			} else if (event.key.keysym.sym == SDLK_s) {
				pad2_state |= PAD_DOWN;
			} else if (event.key.keysym.sym == SDLK_a) {
				pad2_state |= PAD_LEFT;
			} else if (event.key.keysym.sym == SDLK_d) {
				pad2_state |= PAD_RIGHT;
			} else if (event.key.keysym.sym == SDLK_q) {
				pad2_state |= PAD_FIRE;
			} else if (event.key.keysym.sym == SDLK_e) {
				pad2_state |= PAD_PULSE;
			} else if (event.key.keysym.sym == SDLK_ESCAPE) {
				return 3;
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
			} else if (event.key.keysym.sym == SDLK_w) {
				pad2_state &= ~PAD_UP;
			} else if (event.key.keysym.sym == SDLK_s) {
				pad2_state &= ~PAD_DOWN;
			} else if (event.key.keysym.sym == SDLK_a) {
				pad2_state &= ~PAD_LEFT;
			} else if (event.key.keysym.sym == SDLK_d) {
				pad2_state &= ~PAD_RIGHT;
			} else if (event.key.keysym.sym == SDLK_q) {
				pad2_state &= ~PAD_FIRE;
			} else if (event.key.keysym.sym == SDLK_e) {
				pad2_state &= ~PAD_PULSE;
			}
			break;

		default:
			break;
	}

	return 0;
}

void handle_gameplay_updates(void)
{

	if (pad_state & PAD_UP)
		ship_throttle(enterprise);
	if (pad_state & PAD_DOWN)
		ship_break_and_reverse(enterprise);
	if (pad_state & PAD_LEFT)
		ship_rotate_countercw(enterprise);
	if (pad_state & PAD_RIGHT)
		ship_rotate_cw(enterprise);
	if (pad_state & PAD_FIRE) {
		ship_fire_front(enterprise);
	}
	if (pad_state & PAD_PULSE) {
		if (ship_can_pulse(enterprise)) {
			ship_pulse(enterprise);
		}
	}

	if (pad2_state & PAD_UP)
		ship_throttle(enterprise2);
	if (pad2_state & PAD_DOWN)
		ship_break_and_reverse(enterprise2);
	if (pad2_state & PAD_LEFT)
		ship_rotate_countercw(enterprise2);
	if (pad2_state & PAD_RIGHT)
		ship_rotate_cw(enterprise2);
	if (pad2_state & PAD_FIRE) {
		ship_fire_front(enterprise2);
	}
	if (pad2_state & PAD_PULSE) {
		if (ship_can_pulse(enterprise2)) {
			ship_pulse(enterprise2);
		}
	}


	redraw();
	check_collisions();
	update_ships();
	update_lives();
	update_asteroids();
	update_explosions();
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
	label_destroy(score_label1);
	label_destroy(score_label2);
	font_destroy(font);
}

void tear_down_gameplay(void)
{
	free_objects();
}
