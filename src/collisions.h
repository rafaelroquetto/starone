/* 
 * collisions.h - Collision detection functions
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */

#ifndef COLLISIONS_H
#define COLLISIONS_H

/* forward declarations */
struct list;
struct beam;
struct asteroid;
struct ship;

/* callback function type for
 * asteroid-beam collision
 */
typedef void (*ab_callback)(struct asteroid *a,
		struct beam *b);

/* callback function type for
 * asteroid-asteroid collision
 */
typedef void (aa_callback)(struct asteroid *a,
		struct asteroid *b);

/* callback function type for
 * asteroid-ship collision
 */
typedef void (as_callback)(struct asteroid *a,
		struct ship *s);

/* callback function type for
 * ship-ship collision
 */
typedef void (ss_callback)(struct ship *a,
		struct ship *s);

/* iterates over the lists pointed by asteroid_list and ship_list,
 * and checks for asteroid-asteroid and ship-asteroid
 * collisions.
 * In case a collision is found, the related callback 
 * function is called, with the two colliding objects
 * passed as arguments
 * as_cld_callback is called in case the object type == ASTEROID
 * ac_cld_callback is called in case the object type == CRYSTAL
 */
void check_asteroid_collisions(const struct list *asteroid_list, const struct list *ship_list,
		aa_callback aa_cld_callback, as_callback as_cld_callback,
		as_callback ac_cld_callback);

/* iterates over the lists pointed by ship_list and asteroid_list,
 * and checks for beam-asteroid collisions.
 * In case a collision is found, the callback function
 * pointed by 'callback' is called with the two colliding
 * objects as arguments
 */
void check_beam_collisions(const struct list *ship_list,
		const struct list *asteroid_list,
		ab_callback callback);

/* iterates over the list pointed by ship_list
 * and checks for ships-ships collisions.
 * In case a collision is found, calls back the
 * function pointed by 'callback', with colliding
 * ships as arguments
 */
void check_ship_collisions(const struct list *ship_list,
		ss_callback callback);

#endif /* COLLISIONS_H */
