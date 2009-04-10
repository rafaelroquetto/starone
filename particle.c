#include <GL/gl.h>
#include <malloc.h>
#include <math.h>

#include "particle.h"
#include "util.h"

static const float FADE_STEP = 0.03;

struct particle *
particle_new(float x, float y, float accel, float ini_speed,
		float angle, float fade, float radius)
{
	struct particle *p;

	p = malloc(sizeof *p);

	p->x = x;
	p->y = y;
	p->accel = accel;
	p->speed = ini_speed;
	p->angle = angle;
	p->fade = fade;
	p->radius = radius;

	return p;
}

void particle_destroy(struct particle *p)
{
	free(p);
}

void particle_update(struct particle *p)
{
	float rad;

	if (p->speed == 0)
		return;

	rad = deg_to_rad(p->angle);

	p->x += cos(rad)*p->speed;
	p->y += sin(rad)*p->speed;

	p->fade -= FADE_STEP;
	p->speed += p->accel;

	if (p->speed <= 0)
		p->speed = 0;
}

int particle_alive(const struct particle *p)
{
	return (p->fade > 0) ? 1 : 0;
}

void particle_draw(const struct particle *p)
{
	glColor4f(1, 0, 0, p->fade);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(p->x, p->y, 0);
	glRotatef(p->angle, 0, 0, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	glVertex2f(-2.f, -2.f);
	glVertex2f(2.f, -2.f);
	glVertex2f(2.f, 2.f);
	glVertex2f(-2.f, 2.f);

	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();
}
