/*
 *
 */

#include <GL/gl.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>

#include "ship.h"
#include "util.h"
#include "defs.h"
#include "texture.h"
#include "pulse.h"

static const float MOVE_OFFSET = 5.0;
static const float INI_SPEED = 7.0;
static const float MAX_SPEED = 15.0;
static const float SHIP_THROTTLE = 3.0;
static const float SHIP_FRICTION = .5;
static const int INI_BEAM_COUNT = 5;

static GLuint texture;
static int obj_count = 0;


static void
load_texture(void)
{
	assert(obj_count >= 0);

	if (obj_count == 0)
		texture = load_texture_from_png("res/ship.png");
}

static void
delete_texture(void)
{
	assert(obj_count >= 0);

	if (obj_count == 0)
		glDeleteTextures(1, &texture);
}

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
draw_pulses(struct ship *s)
{
	struct node *current;

	current = s->pulse_list->first;

	while (current) {
		pulse_draw((struct pulse *) current->data);
		current = current->next;
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
create_pulse(struct ship *s)
{
	struct pulse *p;

	p = pulse_new(s->x, s->y);

	list_add(s->pulse_list, (void *) p);
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

		if (beam->remove || beam_out_of_bounds(beam,
			WINDOW_WIDTH, WINDOW_HEIGHT)) {

			spare = list_remove(s->beam_list, current);

			beam_destroy(spare);
		}

		current = next;
	}
}

static void
update_pulses(struct ship *s)
{
	struct node *current;
	struct pulse *p;

	current = s->pulse_list->first;

	while (current) {
		struct node *next = current->next;

		p = (struct pulse *) current->data;
		pulse_update(p);

		if (pulse_done(p)) {
			list_remove(s->pulse_list, current);

			pulse_destroy(p);
		}

		current = next;
	}
}

static int
ship_can_fire(const struct ship *s)
{
	return (s->beam_count == 0);
}


struct ship *
ship_new(int x, int y)
{
	struct ship *s;

	s = malloc(sizeof *s);

	ship_init(s, x, y);

	load_texture();

	obj_count++;

	return s;
}

void ship_destroy(struct ship *s)
{
	obj_count--;
	delete_texture();

	list_free(s->beam_list, beam_destroy);
	list_free(s->pulse_list, pulse_destroy);

	free(s);
}

void ship_init(struct ship *s, int x, int y)
{
	s->x = x;
	s->y = y;
	s->angle = rand() % 180;
	s->speed = rand() % (int) MAX_SPEED;
	s->beam_count = INI_BEAM_COUNT;
	s->beam_list = list_new();
	s->pulse_list = list_new();
	s->can_pulse = 1;
	s->accel = 0;

	ship_set_color(s, 1.0, 1.0, 1.0);
}

void ship_set_color(struct ship *s, float r, float g, float b)
{
	s->color[RED] = r;
	s->color[GREEN] = g;
	s->color[BLUE] = b;
}

void ship_draw(const struct ship *s)
{
	glColor3f(s->color[RED], 
		s->color[GREEN],
		s->color[BLUE]);

	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(s->x, s->y, 0);
	glRotatef(s->angle, 0, 0, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(-32.f, -32.f);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(32.f, -32.f);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(32.f, 32.f);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(-32.f, 32.f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	glPopMatrix();

	draw_beams(s);
	draw_pulses(s);
}

void ship_move_forward(struct ship *s)
{
	float rad;

	rad = deg_to_rad(s->angle);

	s->x += cos(rad)*s->speed;
	s->y += sin(rad)*s->speed;
}

void ship_throttle(struct ship *s)
{
	s->speed += SHIP_THROTTLE;

	if (s->speed > MAX_SPEED)
		s->speed = MAX_SPEED;
}

void ship_break_and_reverse(struct ship *s)
{
	s->speed -= SHIP_THROTTLE;

	if (s->speed < -MAX_SPEED)
		s->speed = -MAX_SPEED;
}

void ship_deaccel(struct ship *s)
{
	int signal;

	if (s->speed == 0)
		return;

	signal = (s->speed > 0) ? 1 : -1;

	s->speed -= signal*SHIP_FRICTION;

	if (fabs(s->speed) <= 0)
		s->speed = 0;
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
	ship_move_forward(s);

	ship_deaccel(s);

	if (s->beam_count > 0)
		s->beam_count--;


	update_beams(s);
	update_pulses(s);
}

void ship_fire_front(struct ship *s)
{
	if (ship_can_fire(s)) {
		create_beam(s);

		s->beam_count = INI_BEAM_COUNT;
	}
}

void ship_pulse(struct ship *s)
{
	create_pulse(s);

	s->can_pulse = 0;
}

struct list *
ship_get_beam_list(const struct ship *s)
{
	return s->beam_list;
}

int ship_can_pulse(const struct ship *s)
{
	return (s->can_pulse == 1);
}

void ship_respawn(struct ship *s)
{
	s->x = rand() % WINDOW_WIDTH;
	s->y = rand() % WINDOW_HEIGHT;
	s->speed =  MAX_SPEED;
}
