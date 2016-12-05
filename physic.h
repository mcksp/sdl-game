#ifndef PHYSIC_H
#define PHYSIC_H

#include <SDL.h>
#include "objects.h"

void move_player(struct Player *player); 
SDL_Texture* get_level_texture(SDL_Renderer *renderer);
#endif
