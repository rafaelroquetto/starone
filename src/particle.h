/*
 * particle.h - a simple particle system
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */
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

/* returns a newly created particle,
 * at position (x, y), with acceleration value
 * of 'accel, and initial speed of 'ini_speed',
 * direction angle as in 'angle'
 *
 * NOTE: Accelleration is usually negative
 * The particle shall fade away
 * as it's speed decreases, until it reaches 0 and
 * the particle is considered dead.
 */
struct particle *
particle_new(float x, float y, float accel, float ini_speed,
		float angle);

/* frees the particle from memory, as well
 * as its resources
 */
void particle_destroy(struct particle *p);

/* update particle state
 * i.e., poistion, current color, current
 * speed and so on
 */
void particle_update(struct particle *p);

/* draws the particle to the screen buffer */
void particle_draw(const struct particle *p);

/* sets the particle inital color */
void particle_set_color(struct particle *p, 
		const struct color *c);

/* sets the color to which the particle
 * will merge until it's dead
 */
void particle_fade_to_color(struct particle *p, 
		const struct color *c);

/* returns 1 if particle is alive 
 * (i.e. speed > 0 and still visible)
 * returns 0 otherwise
 */
int particle_alive(const struct particle *p);

#endif /* PARTICLE_H */
