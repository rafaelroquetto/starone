/*
 *
 */

#include <GL/gl.h>
#include <malloc.h>
#include <math.h>

#include "ship.h"
#include "util.h"
#include "defs.h"

static const float MOVE_OFFSET = 5.0;
static const float INI_SPEED = 7.0;
static const int INI_BEAM_COUNT = 5;

extern GLuint ship_texture;

static void
draw_beams(const struct ship *s)
{
	struct node *c;

	c = s->beam_list->first;

	while (c != NULL) {
		beam_draw((struct beam *) c->data);
		c = c->next;
	}
}

static void
create_beam(struct ship *s)
{
	struct beam *b;
	float x0, y0;

	x0 = s->x + SHIP_WIDTH*cos(deg_to_rad(s->angle));
	y0 = s->y + SHIP_WIDTH*sin(deg_to_rad(s->angle));

	b = beam_new(x0, y0, s->angle);

	list_add(s->beam_list, (void *) b);

}

static void
update_beams(struct ship *s)
{
	struct node *current;
	struct beam *beam, *spare;

	current = s->beam_list->first;

	if (current == NULL)
		return;

	current = s->beam_list->first;

	while (current != NULL) {
		struct node *next = current->next;
		beam = (struct beam *) current->data;
		beam_update(beam);

		if (beam_out_of_bounds(beam,
			WINDOW_WIDTH, WINDOW_HEIGHT)) {

			spare = list_remove(s->beam_list, current);

			beam_destroy(spare);
		}

		current = next;
	}
}

struct ship *
ship_new(int x, int y)
{
	struct ship *s;

	s = malloc(sizeof *s);

	ship_init(s, x, y);

	return s;
}

void ship_destroy(struct ship *s)
{
	free(s);
}

void ship_init(struct ship *s, int x, int y)
{
	s->x = x;
	s->y = y;
	s->angle = 0;
	s->speed = INI_SPEED;
	s->beam_count = INI_BEAM_COUNT;
	s->beam_list = list_new();
}

void ship_draw(const struct ship *s)
{
	glColor3f(1, 1, 1);

	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(s->x, s->y, 0);
	glRotatef(s->angle, 0, 0, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ship_texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(-20.f, -20.f);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(15.f, -20.f);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(15.f, 20.f);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(-20.f, 20.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	glPopMatrix();

	draw_beams(s);
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
	s->y -= sin(rad)*s->speed;
}

void ship_rotate_cw(struct ship *s)
{
	s->angle += MOVE_OFFSET;
}

void ship_rotate_countercw(struct ship *s)
{
	s->angle -= MOVE_OFFSET;
}

void ship_update(struct ship *s)
{
	if (s->beam_count > 0)
		s->beam_count--;

	update_beams(s);
}

void ship_fire_front(struct ship *s)
{
	create_beam(s);

	s->beam_count = INI_BEAM_COUNT;
}

int ship_can_fire(const struct ship *s)
{
	return (s->beam_count == 0);
}

struct list *
ship_get_beam_list(const struct ship *s)
{
	return s->beam_list;
}
