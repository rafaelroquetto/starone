#ifndef BEAM_H
#define BEAM_H

struct beam {
	float x;
	float y;

	float angle;

	int remove;
};

struct beam *beam_new(float x, float y, float ang);

void beam_destroy(struct beam *b);
void beam_draw(const struct beam *b);
void beam_update(struct beam *b);
void beam_remove(struct beam *b);

int beam_out_of_bounds(struct beam *b, int w, int h);

#endif /* BEAM_H */
