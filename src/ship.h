/*
 * ship.h - Space ship implementation
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */
#ifndef SHIP_H
#define SHIP_H

#include "beam.h"
#include "list.h"
#include "vector2d.h"

enum 
{
	RED,
	GREEN,
	BLUE,
	RGB
};

struct ship
{
	int x;
	int y;
	int beam_count;
	int can_pulse;
	int lives;

	float angle;
	float speed;
	float accel;
	float radius;
	float color[RGB];

	struct vector2d dir;

	struct list *beam_list;
	struct list *pulse_list;
};

/* returns a new ship
 * at position (x, y)
 */
struct ship *ship_new(int x, int y);

/* destroy the ship and releases
 * any resources to it associated
 */
void ship_destroy(struct ship *s);

/* initializes the ship pointed by s
 * at the position x, y
 */
void ship_init(struct ship *s, int x, int y);

/* sets texture blend color */
void ship_set_color(struct ship *s, float r, float g, float b);

/* draws the ship to the screen buffer */
void ship_draw(const struct ship *s);

/* moves the ship one (as long as accel > 0)
 * step forward
 */
void ship_move_forward(struct ship *s);

/* rotates the ship clockwise */
void ship_rotate_cw(struct ship *s);

/* rotates the ship counterclockwise */
void ship_rotate_countercw(struct ship *s);

/* update the ship state */
void ship_update(struct ship *s);

/* fires the ship front beam */
void ship_fire_front(struct ship *s);

/* accelerates the ship */
void ship_throttle(struct ship *s);

/* breaks and reverses the ship */
void ship_break_and_reverse(struct ship *s);

/* emit a pulse */
void ship_pulse(struct ship *s);

/* respawn the ship at a random
 * position
 */
void ship_respawn(struct ship *s);

/* update ships 'a' supposing it is colliding
 * with ship 'b'
 */
void ship_collide(struct ship *a, struct ship *b);

/* set a new direction angle for the ship */
void ship_set_direction(struct ship *a, float direction);

/* returns 1 if ship is able to emit a pulse
 * returns 0 otherwise
 */
int ship_can_pulse(const struct ship *s);

/* returns a list of beams originated
 * from the ship pointed by s
 */
struct list *
ship_get_beam_list(const struct ship *s);

/*
 * returns ship lives
 */

int ship_get_lives(const struct ship *s);

/* ship size */
#define SHIP_WIDTH 25.f
#define SHIP_HEIGHT  10.f

#endif /* SHIP_H */
