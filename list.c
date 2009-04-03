#include <malloc.h>
#include "list.h"

struct node *
list_push(struct node *h, void *data)
{
	struct node *n;

	n = malloc(sizeof *n);
	n->next = h;
	n->data = data;

	return n;
}

struct node *
list_pop(struct node *h, void **data)
{
	struct node *n;

	n = h->next;

	*data = h->data;

	free(h);

	return n;
}

struct node *
list_remove(struct node *h, struct node *n,
		void **data)
{
	struct node *c;

	if (n == h) {
		c = h->next;

		*data = h->data;
		free(h);

		return c;
	}

	c = h;

	while (c != NULL) {
		if (c->next == n) {
			c->next = n->next;
			*data = n->data;
			free(n);
		}

		c = c->next;
	}

	return h;
}
