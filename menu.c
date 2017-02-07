#include <menu.h>


void server_or_client(SDL_Renderer *renderer, char *menu, TTF_Font *font){
    SDL_Event e;
    int pressed = false;
    while (!pressed) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_c) {
                    *menu = 'c';
                    pressed = true;
                } else if (e.key.keysym.sym == SDLK_s) {
                    *menu = 's';
                    pressed = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(renderer);
        disp_text(renderer, "[s]erver or [c]lient?", font, 240, 200);
        SDL_RenderPresent(renderer);
    }
}


void ask_for_ip(SDL_Renderer *renderer, TTF_Font *font, char *ip) {
    memset(ip, ' ', 15);
    SDL_Event e;
    int position = 0;
    int ok = false;
    while (!ok) {
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) || e.key.keysym.sym == SDLK_PERIOD) {
                    if (position > 14) {
                        position = 14;
                    }
                    ip[position] = e.key.keysym.sym;
                    position++;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    position--;
                    if (position < 0) {
                        position = 0;
                    }
                    ip[position] = ' ';
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    ip[position] = 0;
                    ok = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(renderer);
        disp_text(renderer, "ip addres", font, 240, 200);
        disp_text(renderer, ip, font, 240, 230);
        SDL_RenderPresent(renderer);
    }
}
