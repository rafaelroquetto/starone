#include <malloc.h>

#include "pulse.h"
#include "particle.h"
#include "list.h"
#include "color.h"

enum { N_PARTICLES = 100};

static const float PARTICLE_ACCEL = -0.8;
static const float PARTICLE_SPEED = 12.0;
static const struct color BLUE = { 0.0, 0.7, 1.0, 1.0 };

static void
create_particles(struct pulse *p)
{
	int i;
	float angle, step;

	struct particle *par;

	step = 360.0/(float)N_PARTICLES;
	angle = 0.0;

	for (i = 0; i < N_PARTICLES; i++) {
		par = particle_new(p->x, p->y,
			PARTICLE_ACCEL, PARTICLE_SPEED, angle);

		particle_set_color(par, &BLUE);

		list_add(p->particles, par);

		angle += step;
	}
}

struct pulse *
pulse_new(float x, float y)
{
	struct pulse *p;

	p = malloc(sizeof *p);

	p->x = x;
	p->y = y;
	p->particles = list_new();

	create_particles(p);

	return p;
}

void pulse_update(struct pulse *p)
{
	struct node *current;
	struct particle *par;

	current = p->particles->first;

	while (current) {
		struct node *next = current->next;

		par = (struct particle *) current->data;

		particle_update(par);

		if (!particle_alive(par)) {
			list_remove(p->particles, current);

			particle_destroy(par);
		}

		current = next;
	}
}

void pulse_draw(const struct pulse *p)
{
	struct node *current;

	current = p->particles->first;

	while (current) {
		particle_draw((struct particle *) current->data);
		current = current->next;
	}
}

void pulse_destroy(struct pulse *p)
{
	list_free(p->particles, particle_destroy);
	p->particles = NULL;

	free(p);
}

int pulse_done(const struct pulse *p)
{
	return (list_empty(p->particles)) ? 1 : 0;
}
