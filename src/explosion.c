#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "explosion.h"
#include "list.h"
#include "particle.h"
#include "color.h"
#include "pulse.h"

enum { 
	N_PARTICLES = 350,
	MAX_SPEED = 20,
	MIN_PARTICLES = 40,
	ROUNDS = 4
};

static const float PARTICLE_ACCEL = -0.8;

static struct color COLOR_RED = { 1.0, 0.0, 0.0, 1.0 };
static struct color COLOR_WHITE = { 1.0, 1.0, 1.0, 1.0 };

static void
create_particles(struct explosion *e)
{
	int i, angle, speed;
	struct particle *p;

	srand(time(NULL));

	for (i = 0; i < N_PARTICLES; i++) {
		speed = (rand() % MAX_SPEED) + 1;
		angle = rand() % 360;

		p = particle_new(e->x, e->y, PARTICLE_ACCEL,
			       	speed, angle);
		particle_set_color(p, &COLOR_WHITE);
		particle_fade_to_color(p, &COLOR_RED);

		list_add(e->particles, (void *) p);
	}

	e->rounds--;
}

struct explosion *
explosion_new(float x, float y)
{
	struct explosion *e;

	e = malloc(sizeof *e);

	e->x = x;
	e->y = y;
	e->particles = list_new();
	e->rounds = ROUNDS;
	e->pulse = pulse_new(x, y);

	create_particles(e);

	return e;
}

void explosion_update(struct explosion *e)
{
	struct node *current;
	struct particle *p;

	current = e->particles->first;

	while (current) {
		struct node *next = current->next;

		p = (struct particle *) current->data;

		particle_update(p);

		if (!particle_alive(p)) {
			list_remove(e->particles, current);

			particle_destroy(p);
		}

		current = next;
	}

	if (e->rounds > 0 && list_size(e->particles) > MIN_PARTICLES) {
		create_particles(e);
	}

	if (e->pulse) {
		pulse_update(e->pulse);
	}

	if (e->pulse && pulse_done(e->pulse)) {
		pulse_destroy(e->pulse);
		e->pulse = NULL;
	}
}

void explosion_draw(const struct explosion *e)
{
	struct node *current;

	current = e->particles->first;

	while (current) {
		particle_draw((struct particle *) current->data);
		current = current->next;
	}

	if (e->pulse) {
		pulse_draw(e->pulse);
	}
}

void explosion_destroy(struct explosion *e)
{
	list_free(e->particles, particle_destroy);

	if (e->pulse)
		pulse_destroy(e->pulse);
	e->particles = NULL;

	free(e);
}

int explosion_done(const struct explosion *e)
{
	return (list_empty(e->particles)) ? 1 : 0;
}
