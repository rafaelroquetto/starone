/* 
 * panic.h - simple abortion function
 *
 * Authors:	Rafael Roquetto
 * 		Nicoli Gavassa
 */
#ifndef PANIC_H_
#define PANIC_H_

/* prints the message pointed
 * by fmt, and aborts
 */
void panic(const char *fmt, ...);

#endif /* PANIC_H_ */
