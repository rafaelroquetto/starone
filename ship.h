#ifndef SHIP_H
#define SHIP_H

#include "beam.h"
#include "list.h"

struct ship
{
	int x;
	int y;
	int beam_count;

	float angle;
	float speed;

	struct node *beam_list;
};

struct ship *ship_new(int x, int y);

void ship_destroy(struct ship *s);
void ship_init(struct ship *s, int x, int y);
void ship_draw(const struct ship *s);
void ship_move_forward(struct ship *s);
void ship_move_backwards(struct ship *s);
void ship_rotate_cw(struct ship *s);
void ship_rotate_countercw(struct ship *s);
void ship_update(struct ship *s);
void ship_fire_front(struct ship *s);

int ship_can_fire(const struct ship *s);

struct node *
ship_get_beam_list(const struct ship *s);

#define SHIP_WIDTH 25.f
#define SHIP_HEIGHT  10.f

#endif /* SHIP_H */
