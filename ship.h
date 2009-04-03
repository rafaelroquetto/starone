#ifndef SHIP_H
#define SHIP_H

struct ship
{
	int x;
	int y;

	float angle;
	float speed;
};

struct ship *ship_new();

void ship_destroy(struct ship *s);
void ship_init(struct ship *s);
void ship_draw(const struct ship *s);
void ship_move_forward(struct ship *s);
void ship_move_backwards(struct ship *s);
void ship_rotate_cw(struct ship *s);
void ship_rotate_countercw(struct ship *s);

#define SHIP_WIDTH 25.f
#define SHIP_HEIGHT  10.f

#endif /* SHIP_H */
