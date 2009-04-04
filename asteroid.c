#include <GL/gl.h>
#include <math.h>
#include <stdlib.h>

#include "asteroid.h"
#include "util.h"

static const float ASTEROID_SPEED = 1.f;

struct asteroid *
asteroid_new(float x, float y, float ang)
{
	struct asteroid *a;

	a = malloc(sizeof *a);

	a->x = x;
	a->y = y;
	a->angle = ang;

	return a;
}

void asteroid_destroy(struct asteroid *a)
{
	free(a);
}

void asteroid_draw(struct asteroid *a)
{
	glColor3f(1, 1, 1);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(a->x, a->y, 0);

	glBegin(GL_QUADS);
	glVertex2f(-5.f, -5.f);
	glVertex2f(-5.f, 5.f);
	glVertex2f(5.f, 5.f);
	glVertex2f(5.f, -5.f);
	glEnd();

	glPopMatrix();
}

void asteroid_update(struct asteroid *a)
{
	float rad;

	rad = deg_to_rad(a->angle);

	a->x += cos(rad)*ASTEROID_SPEED;
	a->y += sin(rad)*ASTEROID_SPEED;

}

int asteroid_out_of_bounds(struct asteroid *a, int w, int h)
{
	int obounds;

	if ((a->x < 0) || (a->y < 0) 
		|| (a->x > w) || (a->y > h))
		obounds = 1;
	else
		obounds = 0;

	return obounds;
}

