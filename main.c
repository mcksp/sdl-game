#include <stdio.h>
#include "SDL.h"

#define STEP 4
#define TRUE 1
#define FALSE 0

struct Object {
    SDL_Rect position;
    SDL_Texture *texture;
};

struct Player {
    struct Object object;
    int left, right, up, down;
    int left_key, right_key, up_key, down_key;
};

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

int main(){
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *bitmap = NULL;
    SDL_Texture *tex = NULL;
    struct Player player;
    player.object.position.x = 10;
    player.object.position.y = 10;
    player.object.position.w = 10;
    player.object.position.h = 10;
    player.left_key = SDLK_LEFT;
    player.right_key = SDLK_RIGHT;
    player.up_key = SDLK_UP;
    player.down_key = SDLK_DOWN;
    window = SDL_CreateWindow(
            "game",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            0);

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bitmap = SDL_LoadBMP("xd.bmp");
    tex = SDL_CreateTextureFromSurface(renderer, bitmap);

    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            } 
            resolve_keyboard(e, &player);
        }
        move_player(&player);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, tex, NULL, &player.object.position);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(bitmap);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
