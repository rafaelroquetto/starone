#ifndef COLOR_H
#define COLOR_H

struct color
{
	float red;
	float green;
	float blue;
	float alpha;
};

void color_from_rgba_f(struct color *c, float r, 
		float g, float b, float a);

#endif /* COLOR_H */
