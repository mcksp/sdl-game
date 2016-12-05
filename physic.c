#include "physic.h"

#define TILE_SIZE 32
#define STEP 4

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

void move_and_check_colissions(struct Player *player) {
    if (player->left) {
        player->object.position.x -= STEP;
    }
    if (player->right) {
        player->object.position.x += STEP;
    }
    if (player->up) {
        if (player->in_air == 0) {
            player->in_air = 18;
        }
    }
    if (player->in_air > 0) {
        player->object.position.y -= STEP;
        player->in_air -= 1;
    } else {
        player->object.position.y += STEP;
    }

    struct point left_down, left_up, right_down, right_up;
    int x, y, w, h;
    x = player->object.position.x;
    y = player->object.position.y;
    w = player->object.position.w;
    h = player->object.position.h;
    left_down.y = right_down.y = y + h;
    left_down.x = left_up.x = x;
    right_down.x = right_up.x = x + w;
    right_up.y = left_up.y = y;
    if (map[left_down.y/TILE_SIZE][left_down.x/TILE_SIZE]) {
        player->object.position.y -= (player->object.position.y + w) % TILE_SIZE;
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
    

