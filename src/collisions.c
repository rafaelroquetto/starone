#include "collisions.h"
#include "asteroid.h"
#include "ship.h"
#include "beam.h"
#include "list.h"

void check_asteroid_collisions(const struct list *asteroid_list)
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

static void
check_ship_beam_collisions(struct ship *sh,
	       const struct list *asteroid_list,
	       void (*callback)(struct asteroid *a,
		       struct beam *b))
{
	struct asteroid *asteroid;
	struct beam *beam;

	struct node *b;
	struct node *a;
	struct list *beam_list = ship_get_beam_list(sh);

	for (b = beam_list->first; b; b = b->next) {

		beam = (struct beam *) b->data;

		for (a = asteroid_list->first; a; a = a->next) {

			asteroid = (struct asteroid *) a->data;

			if (beam_hit_asteroid(beam, asteroid)) {
				callback(asteroid, beam);
			}
		}
	}
}

void check_beam_collisions(const struct list *ship_list, 
		const struct list *asteroid_list,
		void (*callback)(struct asteroid *a,
			struct beam *b))
{
	struct ship *sh;
	struct node *a;

	for (a = ship_list->first; a; a = a->next) {
		sh = (struct ship *) a->data;

		check_ship_beam_collisions(sh, asteroid_list, callback);
	}
}

