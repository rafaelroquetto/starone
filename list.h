#ifndef LIST_H
#define LIST_H

struct node
{
	void *data;

	struct node *next;
};

struct node *
list_push(struct node *h, void *data);

struct node *
list_pop(struct node *h, void **data);

struct node *
list_remove(struct node *h, struct node *n, void **data);

#endif /* LIST_H */
