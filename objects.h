#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL.h>


struct Object {
    SDL_Rect position;
    SDL_Texture *texture;
};

struct Player {
    struct Object object;
    int left, right, up, down;
    int left_key, right_key, up_key, down_key;
};

void resolve_player_key_up(int key, struct Player* player);
void resolve_player_key_down(int key, struct Player* player);
void move_player(struct Player*);
void resolve_keyboard(SDL_Event e, struct Player* player);

#endif
