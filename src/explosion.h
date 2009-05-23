#ifndef EXPLOSION_H
#define EXPLOSION_H

struct list;
struct pulse;

struct explosion
{
	float x;
	float y;

	int rounds;

	struct list *particles;

	struct pulse *pulse;
};

struct explosion *
explosion_new(float x, float y);

void explosion_update(struct explosion *e);
void explosion_draw(const struct explosion *e);
void explosion_destroy(struct explosion *e);
int explosion_done(const struct explosion *e);

#endif /* EXPLOSION_H */