#include "physic.h"

#define TILE_SIZE 32
#define STEP 4
#define GRAVITY 1
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

int move_and_check_collisions(SDL_Rect *position, int axis, int mov) {
    SDL_Rect temp = *position;

    if (axis == X_AXIS) {
        temp.x += sign(mov);
    }

    if (axis == Y_AXIS) {
        temp.y += sign(mov);
    }

    if (map[temp.y/TILE_SIZE][temp.x/TILE_SIZE] ||
            map[(temp.y + temp.h)/TILE_SIZE][temp.x/TILE_SIZE] ||
            map[(temp.y)/TILE_SIZE][(temp.x + temp.w)/TILE_SIZE] ||
            map[(temp.y + temp.h)/TILE_SIZE][(temp.x + temp.w)/TILE_SIZE]) {
        return 0;
    } else {
        *position = temp;
        return 1;
    }
}
 
void move_bullets(struct node *bullets) {
    struct node *next = bullets;
    struct Bullet *b;
    while (next != NULL) {
        b = (struct Bullet*) next->data;
        b->position.x += b->face;
        next = next->next;
    }
}

void move_player(struct Player *player) {
    int x_movement = 0;
    int y_movement = 0;
    if (player->left) {
        x_movement -= STEP;
        player->face = -1;
    }
    if (player->right) {
        x_movement += STEP;
        player->face = 1;
    }
    if (player->up) {
        if (player->can_jump) {
            player->can_jump = 0;
            player->y_speed = -25;
        }
    }
    
    y_movement = player->y_speed / 3;
    if (player->y_speed < 25) {
        player->y_speed += GRAVITY;
    }

    while (x_movement != 0 || y_movement != 0) {
        if (x_movement != 0 && move_and_check_collisions(&player->position, X_AXIS, x_movement)) {
            decrement_abs(&x_movement);
        } else {
            x_movement = 0;
        }

        if (y_movement != 0 && move_and_check_collisions(&player->position, Y_AXIS, y_movement)) {
            decrement_abs(&y_movement);
            player->can_jump = 0;
        } else {
            if(y_movement > 0) {
                player->can_jump = 1;
                player->y_speed = 0;
            }
            if(y_movement < 0) {
                player->y_speed = 0;
            }
            y_movement = 0;
        }
    }
}

SDL_Texture* get_map_texture(SDL_Renderer *renderer) {
    SDL_Surface *bitmap = NULL;
    SDL_Texture *map_texture;
    SDL_Rect rect;
    rect.w = 32;
    rect.h = 32;
    bitmap = SDL_LoadBMP("tile.bmp");
    SDL_Texture *tex = NULL;
    tex = SDL_CreateTextureFromSurface(renderer, bitmap);
    map_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
    SDL_SetRenderTarget(renderer, map_texture);
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
    return map_texture;
}
    

