/*
 * explosion.h - particle based explosion
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */
#ifndef EXPLOSION_H
#define EXPLOSION_H

struct list;
struct pulse;

struct explosion
{
	float x;
	float y;

	int rounds;

	struct list *particles;

	struct pulse *pulse;
};

/* returns a newly allocated
 * explosion at (x, y)
 */
struct explosion *
explosion_new(float x, float y);

/* update the explosion state, i.e.,
 * it's pulse and particles positions
 */
void explosion_update(struct explosion *e);

/* draw the explosion to the screen buffer */
void explosion_draw(const struct explosion *e);

/* deallocates the explosion from memory
 * and all of its related resources
 */
void explosion_destroy(struct explosion *e);

/* returns 1 if the explosion is over
 * (i.e., no more visible particles).
 * returns 0 otherwise
 */
int explosion_done(const struct explosion *e);

#endif /* EXPLOSION_H */
