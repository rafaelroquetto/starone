#include <string.h>
#include "panic.h"
#include "label.h"

struct label *
label_new(const char *text, int x, int y,
		struct font *f)
{
	struct label *l;

	l = malloc(sizeof *l);

	l->x = x;
	l->y = y;
	l->font = f;
	l->text = strdup(text);

	return l;
}

void label_draw(struct label *l)
{
	char *text;
	const struct glyph *g;
	float x, y;
	int spacing;

	x = 0;
	y = 0;

	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(l->x, l->y, 0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	text = l->text;

	spacing = l->font->spacing;

	while (*text) {
		if (*text == ' ') {
			x += 3*spacing;

			text++;
		} else {

			g = font_get_glyph(l->font, *text);

			if (g == NULL)
				panic("Cannot load glyph for '%c'.", *text);

			glBindTexture(GL_TEXTURE_2D, g->texture);

			glBegin(GL_QUADS);

			glTexCoord2f(0.0, 0.0);
			glVertex2f(x, y);

			glTexCoord2f(1.0, 0.0);
			glVertex2f(x + g->width, y);

			glTexCoord2f(1.0, 1.0);
			glVertex2f(x + g->width, g->height);

			glTexCoord2f(0.0, 1.0);
			glVertex2f(x, g->height);

			x += g->width + spacing;

			glEnd();

			text++;
		}
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	glPopMatrix();
	SDL_GL_SwapBuffers();
}

void label_destroy(struct label *l)
{
	free(l->text);
	free(l);
}

void label_set_text(struct label *l, const char *text)
{
	free(l->text);
	l->text = strdup(text);
}
