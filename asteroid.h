#ifndef ASTEROID_H
#define ASTEROID_H

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

	int remove;
	int type;

	GLuint *texture;
};

struct asteroid *
asteroid_new(float x, float y, float direction, int type);

void asteroid_destroy(struct asteroid *a);
void asteroid_draw(struct asteroid *a);
void asteroid_update(struct asteroid *a);
void asteroid_remove(struct asteroid *a);
void asteroid_set_direction(struct asteroid *a, float direction);
void asteroid_collide(struct asteroid *a, struct asteroid *b);


int asteroid_out_of_bounds(struct asteroid *a, int w, int h);

#endif /* ASTEROID_H */
