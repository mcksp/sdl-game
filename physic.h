#ifndef PHYSIC_H
#define PHYSIC_H

#include <SDL.h>
#include "objects.h"

void move_and_check_colissions(struct Player *player); 
SDL_Texture* get_level_texture(SDL_Renderer *renderer);
#endif
