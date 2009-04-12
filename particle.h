#ifndef PARTICLE_H
#define PARTICLE_H

#include "color.h"

struct particle
{
	float x;
	float y;
	float accel;
	float speed;
	float ini_speed;
	float angle;
	
	struct color src_color;
	struct color dest_color;
	float alpha;

	int color_fading;
};

struct particle *
particle_new(float x, float y, float accel, float ini_speed,
		float angle);

void particle_destroy(struct particle *p);
void particle_update(struct particle *p);
void particle_draw(const struct particle *p);
void particle_set_color(struct particle *p, 
		const struct color *c);
void particle_fade_to_color(struct particle *p, 
		const struct color *c);

int particle_alive(const struct particle *p);

#endif /* PARTICLE_H */
