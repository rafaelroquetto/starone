#include <GL/gl.h>
#include <malloc.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>

#include "beam.h"
#include "util.h"
#include "texture.h"


static const float BEAM_SPEED = 25.f;

static GLuint texture;
static int obj_count = 0;

static void
load_texture(void)
{
	assert(obj_count >= 0);

	if (obj_count == 0)
		texture = load_texture_from_png("res/beam.png");
}

static void
delete_texture(void)
{
	assert(obj_count >= 0);

	if (obj_count == 0)
		glDeleteTextures(1, &texture);
}

struct beam *
beam_new(float x, float y, float ang)
{
	struct beam *b;

	b = malloc(sizeof *b);
	b->x = x;
	b->y = y;
	b->angle = ang;
	b->remove = 0;

	load_texture();
	obj_count++;

	return b;
}

void beam_destroy(struct beam *b)
{
	obj_count--;

	delete_texture();

	free(b);
}

void beam_draw(const struct beam *b)
{
	glColor3f(1, 1, 1);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(b->x, b->y, 0);
	glRotatef(b->angle, 0, 0, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(0, -8.f);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(32.f, -8.f);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(32.f, 8.f);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(0.0, 8.f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPopMatrix();
}

void beam_update(struct beam *b)
{
	float rad;

	rad = deg_to_rad(b->angle);

	b->x += cos(rad)*BEAM_SPEED;
	b->y += sin(rad)*BEAM_SPEED;	
}

void beam_remove(struct beam *b)
{
	b->remove = 1;
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
