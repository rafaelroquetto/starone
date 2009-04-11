#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "explosion.h"
#include "list.h"
#include "particle.h"

enum { N_PARTICLES = 100 };

static const float PARTICLE_ACCEL = -0.8;
static const float PARTICLE_SPEED = 20;

static void
create_particles(struct explosion *e)
{
	int i, angle;
	struct particle *p;

	srand(time(NULL));

	for (i = 0; i < N_PARTICLES; i++) {
		angle = rand() % 360;

		p = particle_new(e->x, e->y, PARTICLE_ACCEL,
			       	PARTICLE_SPEED, angle);

		list_add(e->particles, (void *) p);
	}
}

struct explosion *
explosion_new(float x, float y)
{
	struct explosion *e;

	e = malloc(sizeof *e);

	e->x = x;
	e->y = y;
	e->particles = list_new();

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

}

void explosion_draw(const struct explosion *e)
{
	struct node *current;

	current = e->particles->first;

	while (current) {
		particle_draw((struct particle *) current->data);
		current = current->next;
	}
}

void explosion_destroy(struct explosion *e)
{
	list_free(e->particles, particle_destroy);
	e->particles = NULL;

	free(e);
}

int explosion_done(const struct explosion *e)
{
	return (list_empty(e->particles)) ? 1 : 0;
}
