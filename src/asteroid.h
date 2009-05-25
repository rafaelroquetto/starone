/*
 * asteroid.h
 * 
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */

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
	/* screen coordinates */
	float x;
	float y;

	/* direction angle */
	float direction;
	float radius;

	/* direction vector */
	struct vector2d dir;

	float speed;

	int remove;
	int type;

	GLuint *texture;
};

/* returns a newly created asteroid
 * at position (x, y), of type 'type',
 * with initial speed of 'speed'.
 * The 'direction' parameter is the value
 * of the angle asteroids forms with
 * the origin (0, 0)
 */
struct asteroid *
asteroid_new(float x, float y, float direction, int type, float speed);

/* deallocs the asteroid and free any
 * resources associated with it
 */
void asteroid_destroy(struct asteroid *a);

/* draws the asteroid to the screen buffer */
void asteroid_draw(struct asteroid *a);

/* update the asteroid state
 * (i.e. position, speed, etc)
 */
void asteroid_update(struct asteroid *a);

/* mark the asteroid to be removed
 * warning: this function does not
 * frees any asteroid-related resource,
 * it justs marks it for lazy (later)
 * removal, i.e., it will be freed
 * on the next update iteration
 */
void asteroid_remove(struct asteroid *a);

/* sets a new direction angle for the asteroid */
void asteroid_set_direction(struct asteroid *a, float direction);

/* callback: updates asteroid pointed by 'a', supposing it is
 * colliding with asteroid pointed by 'b'
 */
void asteroid_collide(struct asteroid *a, const struct asteroid *b);

/* returns the asteroid radius by it's type */
float asteroid_radius_by_type(int type);

/* returns 1 if asteroid is lying outside the 
 * rectangle delimited by (0, 0), (w, 0), (0, h), (w, h)
 * otherwise returns 0
 */
int asteroid_out_of_bounds(struct asteroid *a, int w, int h);

/* returns 1 if asteroid a is colliding
 * with asteroid b
 * returns 0 otherwise
 */
int asteroid_hit_asteroid(const struct asteroid *a,
		const struct asteroid *b);

#endif /* ASTEROID_H */
