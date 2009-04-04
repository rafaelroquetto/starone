#include <GL/gl.h>
#include <malloc.h>
#include <malloc.h>
#include <math.h>
#include "beam.h"
#include "util.h"

static const float BEAM_SPEED = 10.f;

struct beam *
beam_new(float x, float y, float ang)
{
	struct beam *b;

	b = malloc(sizeof *b);
	b->x = x;
	b->y = y;
	b->angle = ang;

	return b;
}

void beam_destroy(struct beam *b)
{
	free(b);
}

void beam_draw(const struct beam *b)
{
	glColor3f(1, 0, 0);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(b->x, b->y, 0);
	glRotatef(b->angle, 0, 0, 1);

	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(10, 0);
	glEnd();

	glPopMatrix();
}

void beam_update(struct beam *b)
{
	float rad;

	rad = deg_to_rad(b->angle);

	b->x += cos(rad)*BEAM_SPEED;
	b->y += sin(rad)*BEAM_SPEED;	
}

int beam_out_of_bounds(struct beam *b, int w, int h)
{
	int obounds;

	if ((b->x < 0) || (b->y < 0) 
		|| (b->x > w) || (b->y > h))
		obounds = 1;
	else
		obounds = 0;

	return obounds;
}
