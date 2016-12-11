#include "objects.h"
#include "physic.h"

#define TRUE 1
#define FALSE 0
#define BULLET_SIZE 8

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
    if (key == player->attack_key) {
        player->shoot = player->face;
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
    if (key == player->attack_key) {
        player->shoot = FALSE;
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
    player->position.x = x;
    player->position.y = y;
}

struct Bullet init_bullet(int x, int y, int face) {
    struct Bullet b;
    b.position.x = x;
    b.position.y = y;
    b.position.w = BULLET_SIZE;
    b.position.h = BULLET_SIZE;
    b.face = face;
    return b;
}
