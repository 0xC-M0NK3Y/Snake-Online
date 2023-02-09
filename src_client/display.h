#ifndef DISPLAY_H
# define DISPLAY_H

#include "structs.h"

void display_map(SDL_Renderer *renderer, const map_t *map);
void display_finish(SDL_Renderer *renderer, param_t *param);

#endif /* display.h */