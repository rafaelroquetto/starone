#include <assert.h>
#include <stdlib.h>

#include "color.h"

void color_from_rgba_f(struct color *c,
	float r, float g, float b, float a)
{
	assert(c != NULL);

	c->red = r;
	c->green = g;
	c->blue = b;
	c->alpha = a;
}
