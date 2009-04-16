#include <GL/gl.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "asteroid.h"
#include "util.h"
#include "texture.h"

static const float ASTEROID_SPEED = 1.f;
static const float MIN_RADIUS = 20.f;
static const float SMALL_RADIUS = 64.0;
static const float LARGE_RADIUS = 64.0;

static GLuint small_texture;
static GLuint large_texture;
static int obj_count = 0;

static void
load_texture(void)
{
	assert(obj_count >= 0);

	if (obj_count == 0) {
		small_texture = load_texture_from_png("res/asteroid3.png");
		large_texture = load_texture_from_png("res/asteroid2.png");
	}
}

static void
delete_texture(void)
{
	assert(obj_count >= 0);

	if (obj_count == 0) {
		glDeleteTextures(1, &small_texture);
		glDeleteTextures(1, &large_texture);
	}
}

struct asteroid *
asteroid_new(float x, float y, float direction, int type)
{
	struct asteroid *a;

	a = malloc(sizeof *a);

	a->x = x;
	a->y = y;
	a->direction = direction;
	a->remove = 0;

	load_texture();

	if (type == ASTEROID_SMALL) {
		a->radius = SMALL_RADIUS;
		a->texture = &small_texture;
	} else if (type == ASTEROID_LARGE) {
		a->radius = LARGE_RADIUS;
		a->texture = &large_texture;
	}

	obj_count++;

	return a;
}

void asteroid_destroy(struct asteroid *a)
{
	obj_count--;

	delete_texture();

	free(a);
}

void asteroid_draw(struct asteroid *a)
{
	GLuint texture = *(a->texture);
	glColor3f(1, 1, 1);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(a->x, a->y, 0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(-a->radius, -a->radius);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(a->radius, -a->radius);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(a->radius, a->radius);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(-a->radius, a->radius);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

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
