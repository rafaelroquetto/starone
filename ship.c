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
static const float INI_SPEED = 5.0;
static const int INI_BEAM_COUNT = 5;


static void
draw_beams(struct ship *s)
{
	struct node *c;

	c = s->beam_list;

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

	s->beam_list = list_push(s->beam_list, (void *) b);

}

static void
update_beams(struct ship *s)
{
	struct node *current;
	struct beam *beam, *spare;

	current = s->beam_list;

	if (current == NULL)
		return;

	while (current) {
		beam = (struct beam *) current->data;
		beam_update(beam);
		
		current = current->next;
	}


	current = s->beam_list;

	while (current != NULL) {
		beam = (struct beam *) current->data;

		if (beam_out_of_bounds(beam,
			WINDOW_WIDTH, WINDOW_HEIGHT)) {

			s->beam_list = list_remove(s->beam_list, current,
					(void *) &spare);

			beam_destroy(spare);
		}

		current = current->next;
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
	s->beam_list = NULL;
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

