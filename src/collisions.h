#ifndef COLLISIONS_H
#define COLLISIONS_H

/* forward declarations */
struct list;
struct beam;
struct asteroid;

void check_asteroid_collisions(const struct list *asteroid_list);
void check_beam_collisions(const struct list *ship_list,
		const struct list *asteroid_list,
	       void (*callback)(struct asteroid *a,
		       struct beam *b));

#endif /* COLLISIONS_H */
