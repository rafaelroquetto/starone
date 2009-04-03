/*
 *
 */

#include <GL/gl.h>
#include <malloc.h>
#include <math.h>

#include "ship.h"
#include "util.h"

static const float MOVE_OFFSET = 5.0;
static const float INI_SPEED = 5.0;

struct ship *
ship_new()
{
	struct ship *s;

	s = malloc(sizeof *s);

	ship_init(s);

	return s;
}

void ship_destroy(struct ship *s)
{
	free(s);
}

void ship_init(struct ship *s)
{
	s->x = 0;
	s->y = 0;
	s->angle = 0;
	s->speed = INI_SPEED;
}

void ship_draw(const struct ship *s)
{
	glColor3f(0, 0, 1);

	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(s->x, s->y, 0);
	glRotatef(s->angle, 0, 0, 1);

	glBegin(GL_TRIANGLES);
	glVertex2f(-10.f, -10.f);
	glVertex2f(15.f, 0.f);
	glVertex2f(-10.f, 10.f);
	glEnd();
	
	glPopMatrix();
}

void ship_move_forward(struct ship *s)
{
	float rad;

	rad = deg_to_rad(s->angle);

	s->x += cos(rad)*s->speed;
	s->y += sin(rad)*s->speed;
}

void ship_move_backwards(struct ship *s)
{
	float rad;

	rad = deg_to_rad(s->angle);

	s->x -= cos(rad)*s->speed;
	s->y -= cos(rad)*s->speed;
}

void ship_rotate_cw(struct ship *s)
{
	s->angle += MOVE_OFFSET;
}

void ship_rotate_countercw(struct ship *s)
{
	s->angle -= MOVE_OFFSET;
}
