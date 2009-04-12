#include <GL/gl.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>

#include "particle.h"
#include "util.h"
#include "texture.h"

static const float FADE_STEP = 0.03;
static const float FADE_FACTOR = 1;
static const float LIFE_FACTOR = 1e-5;
static const float RADIUS = 8.0;
static const float COLOR_FADE_FACTOR = 0.035;

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
	p->alpha = 1.0;
	p->r = 1.0;
	p->g = 1.0;
	p->b = 1.0;
	p->color_fading = 0;

	load_texture();
	obj_count++;

	return p;
}

void particle_destroy(struct particle *p)
{

	obj_count--;
	delete_texture();

	free(p);
}

void particle_set_color(struct particle *p, 
		float r, float g, float b)
{
	p->r = r;
	p->g = g;
	p->b = b;
}

void particle_fade_to_color(struct particle *p,
		float r, float g, float b)
{
	p->color_fading = 1;

	p->rf = r;
	p->gf = g;
	p->bf = b;
}

void particle_update(struct particle *p)
{
	float rad;

	if (p->speed == 0)
		return;

	rad = deg_to_rad(p->angle);

	p->x += cos(rad)*p->speed;
	p->y += sin(rad)*p->speed;

	p->alpha = p->speed/(p->ini_speed + p->ini_speed*FADE_FACTOR);
	p->speed += p->accel;

	/* FIXME: this is not working
	 * replace COLOR_FADE_FACTOR with
	 * dynamic factor
	 * fade color is wrong
	 */
	if (p->color_fading) {
		if (p->r < p->rf)
			p->r += COLOR_FADE_FACTOR;
		else
			p->r -= COLOR_FADE_FACTOR;

		if (p->g < p->gf)
			p->g += COLOR_FADE_FACTOR;
		else
			p->g -= COLOR_FADE_FACTOR;

		if (p->b < p->bf)
			p->b += COLOR_FADE_FACTOR;
		else
			p->b -= COLOR_FADE_FACTOR;
	}

	if (p->speed <= LIFE_FACTOR)
		p->speed = 0;
}

int particle_alive(const struct particle *p)
{
	return (int) p->speed;
}

void particle_draw(const struct particle *p)
{
	glColor4f(p->r, p->g, p->b, p->alpha);

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
	glVertex2f(-RADIUS, -RADIUS);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(RADIUS, -RADIUS);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(RADIUS, RADIUS);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(-RADIUS, RADIUS);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPopMatrix();
}
