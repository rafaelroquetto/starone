#ifndef COLLISIONS_H
#define COLLISIONS_H

/* forward declarations */
struct list;
struct beam;
struct asteroid;
struct ship;

typedef void (*ab_callback)(struct asteroid *a,
		struct beam *b);
typedef void (aa_callback)(struct asteroid *a,
		struct asteroid *b);
typedef void (as_callback)(struct asteroid *a,
		struct ship *s);

void check_asteroid_collisions(const struct list *asteroid_list, const struct list *ship_list,
		aa_callback aa_cld_callback, as_callback as_cld_callback);

void check_beam_collisions(const struct list *ship_list,
		const struct list *asteroid_list,
		ab_callback callback);

#endif /* COLLISIONS_H */
