#include <GL/gl.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "asteroid.h"
#include "util.h"
#include "texture.h"

static const float MIN_RADIUS = 20.f;
static const float SMALL_RADIUS = 54.0;
static const float MEDIUM_RADIUS = 64.0;
static const float LARGE_RADIUS = 74.0;

static const int MAX_MASS = 20;
static const int FUDGE_FACTOR = 100.f;

static GLuint small_texture;
static GLuint large_texture;
static int obj_count = 0;

static void
load_texture(void)
{
	assert(obj_count >= 0);

	if (obj_count == 0) {
		small_texture = load_texture_from_png("res/asteroid.png", NULL, NULL);
		large_texture = load_texture_from_png("res/asteroid2.png", NULL, NULL);
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
asteroid_new(float x, float y, float direction, int type, float speed)
{
	struct asteroid *a;

	a = malloc(sizeof *a);

	a->x = x;
	a->y = y;
	a->direction = direction;
	a->remove = 0;
	a->speed = speed;
	a->type = type;

	asteroid_set_direction(a, direction);

	load_texture();

	if (type == ASTEROID_SMALL) {
		a->radius = SMALL_RADIUS;
	} else if (type == ASTEROID_LARGE) {
		a->radius = LARGE_RADIUS;
	} else if (type == ASTEROID_MEDIUM) {
		a->radius = MEDIUM_RADIUS;
	} else if (type == CRYSTAL) {
		a->radius = SMALL_RADIUS;
	}

	if (type == CRYSTAL) {
		a->texture = &small_texture;
	} else {
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
	a->x += a->dir.x;
	a->y += a->dir.y;
}

int asteroid_out_of_bounds(struct asteroid *a, int w, int h)
{
	float diameter;

	diameter = 2*a->radius;

	return ((a->x < -diameter) || (a->y < -diameter) 
		|| (a->x > w + diameter) || (a->y > h + diameter));
}

void asteroid_remove(struct asteroid *a)
{
	a->remove = 1;
}

void asteroid_set_direction(struct asteroid *a, float direction)
{
	float rad;

	rad = deg_to_rad(direction);

	a->dir.x = cos(rad)*a->speed;
	a->dir.y = sin(rad)*a->speed;
}

void asteroid_collide(struct asteroid *a, const struct asteroid *b)
{
	struct vector2d dir;
	float len;

	dir.x = b->x - a->x;
	dir.y = b->y - a->y;

	len = dir.x*dir.x + dir.y*dir.y;
	
	a->dir.x = -FUDGE_FACTOR*dir.x/(1 + len);
	a->dir.y = -FUDGE_FACTOR*dir.y/(1 + len);
}

float asteroid_radius_by_type(int type)
{
	if (type == ASTEROID_SMALL)
		return SMALL_RADIUS;
	else if(type == ASTEROID_LARGE)
		return LARGE_RADIUS;
	else
		assert(0);
}

int asteroid_hit_asteroid(const struct asteroid *a,
		const struct asteroid *b)
{
	float d_square;
	float r_square;
	float dx = (b->x - a->x);
	float dy = (b->y - a->y);

	d_square = dx*dx + dy*dy;

	r_square = a->radius;
	r_square *= r_square;

	return (d_square < r_square);
}

int asteroid_type(const struct asteroid *a)
{
	assert(a);
	return a->type;
}

