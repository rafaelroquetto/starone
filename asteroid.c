#include <GL/gl.h>
#include <math.h>
#include <stdlib.h>

#include "asteroid.h"
#include "util.h"

static const float ASTEROID_SPEED = 1.f;
static const int NUM_STEPS = 10;
static const float MIN_RADIUS = 6.f;

struct asteroid *
asteroid_new(float x, float y, float radius, float direction)
{
	struct asteroid *a;

	a = malloc(sizeof *a);

	a->x = x;
	a->y = y;
	a->radius = (radius > MIN_RADIUS) ? radius : MIN_RADIUS;
	a->direction = direction;
	a->remove = 0;

	return a;
}

void asteroid_destroy(struct asteroid *a)
{
	free(a);
}

void asteroid_draw(struct asteroid *a)
{
	int i;
	float b = 0;
	float db = 2.*M_PI/NUM_STEPS;

	glColor3f(1, 1, 1);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(a->x, a->y, 0);

	glBegin(GL_POLYGON);

	for (i = 0; i < NUM_STEPS; i++) {
		float x = a->radius*cos(b);
		float y = a->radius*sin(b);

		glVertex2f(x, y);

		b += db;
	}

	glEnd();

	glPopMatrix();
}

void asteroid_update(struct asteroid *a)
{
	float rad;

	rad = deg_to_rad(a->direction);

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

void asteroid_remove(struct asteroid *a)
{
	a->remove = 1;
}
