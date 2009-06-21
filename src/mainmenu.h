#ifndef MAINMENU_H
#define MAINMENU_H
#include <SDL.h>

void handle_mainmenu_updates(void);
void tear_down_mainmenu(void);
void initialize_mainmenu_data(void);

int handle_mainmenu_events(SDL_Event event);

#endif /* MAINMENU_H */
