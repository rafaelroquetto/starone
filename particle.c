#include <GL/gl.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>

#include "particle.h"
#include "util.h"
#include "texture.h"

static const float FADE_STEP = 0.03;
static const float FADE_FACTOR = 4;
static const float LIFE_FACTOR = 1e-5;
static const int COLOR_MOD = 255;

static GLuint texture;
static int obj_count = 0;

static void
load_texture(void)
{
	assert(obj_count >= 0);

	if (obj_count == 0)
		texture = load_texture_from_png("res/particle.png");
}

static void
delete_texture(void)
{
	assert(obj_count >= 0);

	if (obj_count == 0)
		glDeleteTextures(1, &texture);
}

struct particle *
particle_new(float x, float y, float accel, float ini_speed,
		float angle)
{
	struct particle *p;

	p = malloc(sizeof *p);

	p->x = x;
	p->y = y;
	p->accel = accel;
	p->speed = ini_speed;
	p->ini_speed = ini_speed;
	p->angle = angle;
	p->fade = 1.0;

	load_texture();
	obj_count++;

	return p;
}

void particle_destroy(struct particle *p)
{

	obj_count--;
	delete_texture();

//	free(p);
}

void particle_update(struct particle *p)
{
	float rad;

	if (p->speed == 0)
		return;

	rad = deg_to_rad(p->angle);

	p->x += cos(rad)*p->speed;
	p->y += sin(rad)*p->speed;

	p->fade = p->speed/(p->ini_speed + p->ini_speed*FADE_FACTOR);
	p->speed += p->accel;

	if (p->speed <= 0)
		p->speed = 0;
}

int particle_alive(const struct particle *p)
{
	return (p->fade > LIFE_FACTOR) ? 1 : 0;
}

void particle_draw(const struct particle *p)
{
	glColor4f(1, 1, 1, p->fade);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(p->x, p->y, 0);
	glRotatef(p->angle, 0, 0, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(-8.0, -8.0);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(8.0, -8.0);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(8.0, 8.0);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(-8.0, 8.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPopMatrix();
}
