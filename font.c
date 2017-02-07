#include <font.h>

void disp_text(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y) {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect pos;
    SDL_Color color = {255, 255, 255};
    surface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    pos.w = surface->w;
    pos.h = surface->h;
    pos.x = x;
    pos.y = y;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &pos);
}
