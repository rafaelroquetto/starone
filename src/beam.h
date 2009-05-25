/*
 * beam.h
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */

#ifndef BEAM_H
#define BEAM_H

struct beam {
	float x;
	float y;

	float angle;

	int remove;
};

/* returns a newly created
 * beam at position (x, y) with
 * a direction angle value of 'ang'
 */
struct beam *beam_new(float x, float y, float ang);

/* frees the beam pointed by b
 * and any related resources 
 */
void beam_destroy(struct beam *b);

/* draws the beam to the screen buffer */
void beam_draw(const struct beam *b);

/* updates beam state (position) */
void beam_update(struct beam *b);

/* marks beam for lazy removal */
void beam_remove(struct beam *b);

/* returns 1 if the beam poitned by *b
 * lies outside the rectangle delimited by
 * (0, 0), (w, 0), (0, h), (w, h)
 * returns 0 otherwise
 */
int beam_out_of_bounds(struct beam *b, int w, int h);

#endif /* BEAM_H */
