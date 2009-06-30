#ifndef LABEL_H
#define LABEL_H

#include "font.h"

struct label
{
	struct font *font;
	char *text;

	int x;
	int y;
};

struct label *
label_new(const char *text, int x, int y,
		struct font *f);

void label_draw(struct label *l);
void label_destroy(struct label *l);
void label_set_text(struct label *l, const char *text);

#endif /* LABEL_H */
