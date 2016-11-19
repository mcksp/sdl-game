#include <stdio.h>
#include <SDL.h>
#include "objects.h"
#include "client_udp.h"

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

    prepare_client();
    send_to_server(422.2, 3.2);

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
