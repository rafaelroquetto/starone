#include "collisions.h"
#include "asteroid.h"
#include "ship.h"
#include "beam.h"
#include "list.h"

static int
asteroid_hit_ship(const struct asteroid *a,
		const struct ship *s)
{
	float d_square;
	float r_square;
	float dx = (s->x - a->x);
	float dy = (s->y - a->y);

	d_square = dx*dx + dy*dy;

	r_square = a->radius;
	r_square *= r_square;

	return (d_square < r_square);
}

void check_asteroid_collisions(const struct list *asteroid_list,
		const struct list *ship_list,
		aa_callback aa_cld_callback, as_callback as_cld_callback)
{
	struct node *a;
	struct node *b;
	struct node *s;
	struct asteroid *current;
	struct asteroid *iterator;
	struct ship *sh;

	for (a = asteroid_list->first; a; a = a->next) {
		current = (struct asteroid *) a->data;

		/* check for ship collisions */
		for (s = ship_list->first; s; s = s->next) {
			sh = (struct ship *) s->data;

			if (asteroid_hit_ship(current, sh)) {
				as_cld_callback(current, sh);
			}
		}

		for (b = asteroid_list->first; b; b = b->next) {
			iterator = (struct asteroid *) b->data;

			if (iterator == current)
				continue;

			if (asteroid_hit_asteroid(current, iterator)) {
				aa_cld_callback(current, iterator);
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
	       ab_callback callback)
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

static int
ships_collide(const struct ship *a, const struct ship *b)
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

void check_ship_collisions(const struct list *ship_list,
		ss_callback callback)
{
	struct node *current;
	struct node *iterator;
	struct ship *current_ship;
	struct ship *iter_ship;

	for (current = ship_list->first; current; current = current->next) {
		current_ship = (struct ship *) current->data;

		for (iterator = ship_list->first; iterator; iterator = iterator->next) {
			iter_ship = (struct ship *) iterator->data;

			if (iter_ship == current_ship)
				continue;

			if (ships_collide(iter_ship, current_ship))
				callback(iter_ship, current_ship);
		}
	}
}
