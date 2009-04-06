#include <stdlib.h>
#include <stdio.h>
#include "list.h"


void dump_list(const struct list *l)
{
	struct node *n = l->first;
	while (n) {
		printf("n == %p\n", n);
		printf("n->data == %p\n", n->data);
		printf("n->prev == %p\n", n->prev);
		printf("n->next == %p\n", n->next);
		printf("-----\n");
		n = n->next;
	}

	putchar('\n');
}

struct list *
list_new()
{
	struct list *l;

	l = malloc(sizeof *l);

	l->first = NULL;

	return l;
}

void
list_add(struct list *l, void *data)
{
	struct node *n;
	struct node *last;

	n = malloc(sizeof *n);
	n->data = data;

	n->next = l->first;
	n->prev = NULL;

	if (l->first)
		l->first->prev = n;

	l->first = n;

}

void *
list_remove(struct list *l, struct node *n)
{
	void *data = n->data;
	if (n->prev) {
		n->prev->next = n->next;
	} else {
		l->first = n->next;
	}

	if (n->next) {
		n->next->prev = n->prev;
	}

	free (n);

	return data;
}

