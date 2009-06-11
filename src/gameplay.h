#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include <SDL.h>
void handle_gameplay_updates(void);
void tear_down_gameplay(void);
void initialize_gameplay_data(void);

int handle_gameplay_events(SDL_Event event);

#endif /* GAMEPLAY_H */
