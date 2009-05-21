#ifndef PULSE_H
#define PULSE_H

struct list;

struct pulse
{
	float x;
	float y;

	struct list *particles;
};

struct pulse *
pulse_new(float x, float y);

void pulse_update(struct pulse *p);
void pulse_draw(const struct pulse *p);
void pulse_destroy(struct pulse *p);

int pulse_done(const struct pulse *p);

#endif /* PULSE_H */
