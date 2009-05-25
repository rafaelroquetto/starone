/*
 * color.h - simple rgba color structure
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */
#ifndef COLOR_H
#define COLOR_H

struct color
{
	float red;
	float green;
	float blue;
	float alpha;
};

/* fills in the struct color pointed by 'c',
 * with the values passed to the r, g, b and a
 * parameters
 */
void color_from_rgba_f(struct color *c, float r, 
		float g, float b, float a);

#endif /* COLOR_H */
