#include "physic.h"

#define TILE_SIZE 32
#define STEP 4
#define X_AXIS 1
#define Y_AXIS 0

struct point {
    int x;
    int y;
};

int map[15][20] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0},
    {0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

int sign(int a) {
    if (a < 0) {
        return -1;
    } else if (a > 0) {
        return 1;
    } else {
        return 0;
    }
}

void decrement_abs(int *a) {
    *a -= sign(*a);
}

int move_and_check_collisions(struct Player *player, int axis, int mov) {
    SDL_Rect pos = player->position;

    if (axis == X_AXIS) {
        pos.x += sign(mov);
    }

    if (axis == Y_AXIS) {
        pos.y += sign(mov);
    }

    if (map[pos.y/TILE_SIZE][pos.x/TILE_SIZE] ||
            map[(pos.y + pos.h)/TILE_SIZE][pos.x/TILE_SIZE] ||
            map[(pos.y)/TILE_SIZE][(pos.x + pos.w)/TILE_SIZE] ||
            map[(pos.y + pos.h)/TILE_SIZE][(pos.x + pos.w)/TILE_SIZE]) {
        return 0;
    } else {
        player->position = pos;
        return 1;
    }
}
        

void move_player(struct Player *player) {
    int x_movement = 0;
    int y_movement = 0;
    if (player->left) {
        x_movement -= STEP;
    }
    if (player->right) {
        x_movement += STEP;
    }
    if (player->up) {
        if (player->in_air == 0) {
            player->in_air = 18;
        }
    }

    if (player->in_air > 0) {
        y_movement -= STEP;
        player->in_air -= 1;
    } else {
        y_movement += STEP;
    }

    while (x_movement != 0 || y_movement != 0) {
        if (x_movement != 0 && move_and_check_collisions(player, X_AXIS, x_movement)) {
            decrement_abs(&x_movement);
        } else {
            x_movement = 0;
        }

        if (y_movement != 0 && move_and_check_collisions(player, Y_AXIS, y_movement)) {
            decrement_abs(&y_movement);
        } else {
            y_movement = 0;
        }
    }
}

SDL_Texture* get_level_texture(SDL_Renderer *renderer) {
    SDL_Surface *bitmap = NULL;
    SDL_Texture *level;
    SDL_Rect rect;
    rect.w = 32;
    rect.h = 32;
    bitmap = SDL_LoadBMP("tile.bmp");
    SDL_Texture *tex = NULL;
    tex = SDL_CreateTextureFromSurface(renderer, bitmap);
    level = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    SDL_SetRenderTarget(renderer, level);
    int i, j;
    for (i = 0; i < 480 / TILE_SIZE; i++) {
        for (j = 0; j < 640 / TILE_SIZE; j++) {
            if (map[i][j]) {
                rect.x = TILE_SIZE * j;
                rect.y = TILE_SIZE * i;
                SDL_RenderCopy(renderer, tex, NULL, &rect);
            }
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
    return level;
}
    

