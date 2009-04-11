#ifndef LIST_H
#define LIST_H

struct node
{
	void *data;

	struct node *next;
	struct node *prev;
};

struct list
{
	struct node *first;
};

struct list *
list_new(void);

void list_add(struct list *l, void *data);
void *list_remove(struct list *l, struct node *n);
void list_free(struct list *l, void (*free_func(void *)));

int list_empty(const struct list *l);

#endif /* LIST_H */
