#include <stdio.h>
#include <SDL.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include "objects.h"
#include "client_udp.h"
#include "server_udp.h"
#include "network.h"

#define MAX_PLAYERS 10

struct Player players[MAX_PLAYERS];
int number_of_players = 0;
int16_t my_id = -1;

void init_players() {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
        players[i].object.position.x = 10 + (60 * i);
        players[i].object.position.y = 10 + (60 * i);
        players[i].object.position.w = 10 * (i + 1);
        players[i].object.position.h = 10 * (i + 1);
        players[i].left_key = SDLK_LEFT;
        players[i].right_key = SDLK_RIGHT;
        players[i].up_key = SDLK_UP;
        players[i].down_key = SDLK_DOWN;
    }
}

void receive_new_id(int id) {
    my_id = id;
    number_of_players = id;
    printf("my_id is now: %d\n", my_id);
}

void check_if_its_new_player(int id){
    if (id > number_of_players) {
        number_of_players = id;
        printf("new max player, now %d\n", number_of_players + 1);
    }
}


void* client_loop(void *arg) {
    int socket = *((int *) arg);
    while (1) {
        int id, x, y;
        client_listen(socket, &id, &x, &y);
        if (id == -1) {
            receive_new_id(x);
        }
        if (id >= 0) {
            check_if_its_new_player(id);
            players[id].object.position.x = x;
            players[id].object.position.y = y;
        }
        usleep(50);
    }
}

int main(){
    struct sockaddr_in server_addr, client_addr;
    int sock_server, sock_client;
    server_addr = server_sock_addr();
    client_addr = client_sock_addr();
    
    char menu;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface *bitmap = NULL;
    SDL_Texture *tex = NULL;
    init_players();
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
    int i;
    printf("[s]erver or [c]lient?\n");
    scanf("%c", &menu);
    pthread_t thread_id_server, thread_id_client, thread_id_server_send;
    if(menu == 's') {
        prepare_server(&sock_server, &server_addr);
        pthread_create(&thread_id_server, NULL, server_receive_loop, &sock_server);
        pthread_create(&thread_id_server_send, NULL, server_send_loop, &sock_server);
    }
    prepare_client(&sock_client, &client_addr);
    pthread_create(&thread_id_client, NULL, client_loop, &sock_client);

    while (my_id < 0) {
        send_to_server(sock_client, server_addr, my_id, 0, 0);
        usleep(100);
    }

    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
            resolve_keyboard(e, &players[my_id]);
        }
        move_player(&players[my_id]);
        send_to_server(sock_client, server_addr, my_id, players[my_id].object.position.x, players[my_id].object.position.y);

        SDL_RenderClear(renderer);
        for (i = 0; i <= number_of_players; i++) {
            SDL_RenderCopy(renderer, tex, NULL, &players[i].object.position);
        }
        SDL_RenderPresent(renderer);
    }
   

    close(sock_client);
    close(sock_server);
    pthread_cancel(thread_id_client);
    pthread_cancel(thread_id_server);
    pthread_cancel(thread_id_server_send);
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(bitmap);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
