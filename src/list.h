/*
 * list.h - doubly linked list
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */
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
	size_t size;
	struct node *first;
};

/* returns a newly created
 * empty list
 */
struct list *
list_new(void);

/* add the contents pointed by
 * data to the list
 * Note: the contents aren't copied,
 * but simply referenced by the list
 */
void list_add(struct list *l, void *data);

/* removes and destroys (frees) the node
 * pointed by n, from the list, and returns
 * it's data contents for later manipulation
 */
void *list_remove(struct list *l, struct node *n);

/* removes and destroy all elements from the list,
 * including the list itself
 * for each element on the list, free_func
 * will be called in order to free their memory
 */
void list_free(struct list *l, void (*free_func(void *)));

/* returns 1 if list is empty
 * returns 0 otherwise
 */
int list_empty(const struct list *l);

/* returns the numebr of elements
 * inside the list
 */
int list_size(const struct list *l);

#endif /* LIST_H */
