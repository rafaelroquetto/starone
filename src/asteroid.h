#ifndef ASTEROID_H
#define ASTEROID_H

#include "vector2d.h"
#include <GL/gl.h>


enum asteroid_type
{
	ASTEROID_SMALL,
	ASTEROID_LARGE
};


struct asteroid
{
	float x;
	float y;

	float direction;
	float radius;

	struct vector2d dir;

	float speed;

	int remove;
	int type;

	GLuint *texture;
};

struct asteroid *
asteroid_new(float x, float y, float direction, int type, float speed);

void asteroid_destroy(struct asteroid *a);
void asteroid_draw(struct asteroid *a);
void asteroid_update(struct asteroid *a);
void asteroid_remove(struct asteroid *a);
void asteroid_set_direction(struct asteroid *a, float direction);
void asteroid_collide(struct asteroid *a, const struct asteroid *b);

float asteroid_radius_by_type(int type);

int asteroid_out_of_bounds(struct asteroid *a, int w, int h);
int asteroid_hit_asteroid(const struct asteroid *a,
		const struct asteroid *b);

#endif /* ASTEROID_H */
