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
struct Player players_server[MAX_PLAYERS];
int number_of_connected_clients = 0;
int number_of_players = 0;
struct sockaddr_in clients_addresses[MAX_PLAYERS];
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



void* server_receive_loop(void *arg) {
    int socket = *((int *) arg);
    int client_pos = 0;
    struct sockaddr_in client_addr;
    int16_t tab[3];

    while (1) {
        client_addr = receive_data(socket, tab);

        client_pos = addr_pos_in_tab(client_addr, clients_addresses, number_of_connected_clients);
        if (client_pos < number_of_connected_clients && client_pos >= 0) {
            players_server[client_pos].object.position.x = tab[1];
            players_server[client_pos].object.position.y = tab[2];
        }
        if (client_pos >= number_of_connected_clients && client_pos < MAX_PLAYERS) {
            clients_addresses[number_of_connected_clients++] = client_addr;
        }
        if (tab[0] == -1) {
            printf("client_pos when -1 in server_receive %d\n", client_pos);
            int16_t tab[3];
            tab[0] = -1;
            tab[1] = client_pos;
            send_data(socket, clients_addresses[client_pos], tab);
        }
        usleep(50);
    }
}

void* server_send_loop(void *arg) {
    int socket = *((int *) arg);
    int16_t tab[3];
    while (1) {
        int i, j;
        for (i = 0; i < number_of_connected_clients; i++) {
            for (j = 0; j < number_of_connected_clients; j++) {
                if (i != j) {
                    tab[0] = j;
                    tab[1] = players_server[j].object.position.x;
                    tab[2] = players_server[j].object.position.y;
                    send_data(socket, clients_addresses[i], tab);
                }
            }
        }
        usleep(100);
    }
}

void* client_loop(void *arg) {
    int socket = *((int *) arg);
    while (1) {
        int id, x, y;
        client_listen(socket, &id, &x, &y);
        if (id == -1) {
            my_id = x;
            number_of_players = x;
            printf("my_id is now: %d\n", my_id);
        }
        if (id >= 0) {
            players[id].object.position.x = x;
            players[id].object.position.y = y;
            if (id > number_of_players) {
                number_of_players = id;
                printf("new max player, now %d\n", number_of_players + 1);
            }
        }
        usleep(50);
    }
}

int main(){
    memset(clients_addresses, 0, sizeof(struct sockaddr_in) * MAX_PLAYERS);
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

    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
            if (my_id >= 0) {
                resolve_keyboard(e, &players[my_id]);
            }
        }
        if (my_id >= 0) {
            move_player(&players[my_id]);
            send_to_server(sock_client, server_addr, my_id, players[my_id].object.position.x, players[my_id].object.position.y);
        } else {
            send_to_server(sock_client, server_addr, my_id, 0, 0);
        }

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
