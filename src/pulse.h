/* 
 * pulse.h - A particle based pulse implementation
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */
#ifndef PULSE_H
#define PULSE_H

struct list;

struct pulse
{
	float x;
	float y;

	struct list *particles;
};

/* creates a new pulse
 * at (x, y)
 */
struct pulse *
pulse_new(float x, float y);

/* updates pulse state */
void pulse_update(struct pulse *p);

/* draw it to the screen buffer */
void pulse_draw(const struct pulse *p);

/* releases and destroyes the pulse */
void pulse_destroy(struct pulse *p);

/* returns 1 if pulse is done,
 * i.e. not visible
 * returns 0 otherwise
 */
int pulse_done(const struct pulse *p);

#endif /* PULSE_H */
