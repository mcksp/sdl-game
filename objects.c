#include "objects.h"

#define TRUE 1
#define FALSE 0
#define STEP 4

void resolve_player_key_down(int key, struct Player* player) {
    if (key == player->left_key) {
        player->left = TRUE;
    }
    if (key == player->right_key) {
        player->right = TRUE;
    }
    if (key == player->up_key) {
        player->up = TRUE;
    }
    if (key == player->down_key) {
        player->down = TRUE;
    }
}

void resolve_player_key_up(int key, struct Player* player) {
    if (key == player->left_key) {
        player->left = FALSE;
    }
    if (key == player->right_key) {
        player->right = FALSE;
    }
    if (key == player->up_key) {
        player->up = FALSE;
    }
    if (key == player->down_key) {
        player->down = FALSE;
    }
}


void move_player(struct Player* player) {
    if(player->left) {
        player->object.position.x -= STEP;
    }
    if(player->right) {
        player->object.position.x += STEP;
    }
    if(player->up) {
        player->object.position.y -= STEP;
    }   
    if(player->down) {
        player->object.position.y += STEP;
    }
}

void resolve_keyboard(SDL_Event e, struct Player* player) {
    if (e.type == SDL_KEYDOWN) {
        resolve_player_key_down(e.key.keysym.sym, player);
    } 
    if (e.type == SDL_KEYUP) {
        resolve_player_key_up(e.key.keysym.sym, player);
    }
}

void set_player_pos(struct Player* player, float x, float y) {
    player->object.position.x = x;
    player->object.position.y = y;
}


