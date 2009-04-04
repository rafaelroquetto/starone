#ifndef ASTEROID_H
#define ASTEROID_H

struct asteroid
{
	float x;
	float y;

	float angle;
};

struct asteroid *
asteroid_new(float x, float y, float angle);

void asteroid_destroy(struct asteroid *a);
void asteroid_draw(struct asteroid *a);
void asteroid_update(struct asteroid *a);

int asteroid_out_of_bounds(struct asteroid *a, int w, int h);

#endif /* ASTEROID_H */
