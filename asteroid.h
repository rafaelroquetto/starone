#ifndef ASTEROID_H
#define ASTEROID_H

struct asteroid
{
	float x;
	float y;

	float direction;
	float radius;

	int remove;
};

struct asteroid *
asteroid_new(float x, float y, float radius, float direction);

void asteroid_destroy(struct asteroid *a);
void asteroid_draw(struct asteroid *a);
void asteroid_update(struct asteroid *a);
void asteroid_remove(struct asteroid *a);
void asteroid_load_texture(void);

int asteroid_out_of_bounds(struct asteroid *a, int w, int h);

#endif /* ASTEROID_H */
