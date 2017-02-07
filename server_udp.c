#include "server_udp.h"
#include "network.h"
#include "objects.h"
#include "list.h"
#include "physic.h"
#include "constans.h"
#include "time.h"
#include "sys/time.h"

struct sockaddr_in clients_addresses[MAX_PLAYERS];
struct Player players_server[MAX_PLAYERS];
struct node *bullets_server = NULL;
int number_of_connected_clients = 0;

void prepare_server(int *sock, struct sockaddr_in *server_sock) {
    memset(clients_addresses, 0, sizeof(struct sockaddr_in) * MAX_PLAYERS);
    if ((*sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket failed");
    }
    if (bind(*sock, (struct sockaddr*) server_sock, sizeof(struct sockaddr)) < 0) {
        perror("bind server error");
    }
}

struct sockaddr_in receive_data(int sock, int16_t data[]) {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr);
    recvfrom(sock, data, sizeof(int16_t) * 4, 0, (struct sockaddr*)&addr, &addr_size);
    return addr;
}

void send_data(int sock, struct sockaddr_in client, int16_t data[], int size) {
    socklen_t addr_size = sizeof(struct sockaddr);
    sendto(sock, data, sizeof(int16_t) * size, 0, (struct sockaddr*)&client, addr_size);
}

void init_players_tab() {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
        players_server[i].position.w = PLAYER_WIDTH;
        players_server[i].position.h = PLAYER_HEIGHT;
        players_server[i].position.x = SPAWN_X;
        players_server[i].position.y = SPAWN_Y;
    }
}

void* server_receive_loop(void *arg) {
    int socket = *((int *) arg);
    int client_pos = 0;
    struct sockaddr_in client_addr;
    int16_t tab[4];
    init_players_tab();
    while (1) {
        client_addr = receive_data(socket, tab);
        client_pos = addr_pos_in_tab(client_addr, clients_addresses, number_of_connected_clients);
        if (its_an_old_client(client_pos)) {
            int16_t keys = tab[1];
            player_from_key_state(&players_server[client_pos], keys);
            if(players_server[client_pos].shoot && !players_server[client_pos].reloading) {
                struct Bullet temp;
                temp.position.x = players_server[client_pos].position.x;
                temp.position.y = players_server[client_pos].position.y;
                temp.position.w = BULLET_WIDTH;
                temp.position.h = BULLET_HEIGHT;
                temp.face = players_server[client_pos].face;
                if (temp.face == 1) {
                    temp.position.x += PLAYER_WIDTH;
                } else {
                    temp.position.x -= BULLET_WIDTH;
                }
                temp.player_id = client_pos;
                push_element(&bullets_server, &temp, sizeof(struct Bullet));
            }
            players_server[client_pos].reloading = players_server[client_pos].shoot;
        }
        if (tab[0] == -1 && client_pos < MAX_PLAYERS) {
            add_adr_to_list(client_pos, &client_addr);
            int16_t tab[3];
            tab[0] = -1;
            tab[1] = client_pos;
            send_data(socket, clients_addresses[client_pos], tab, 3);
        }
        usleep(50);
    }
}

int get_bullet_array(struct node *list, int16_t **array) {
    int n = 0;
    struct node *temp = list;
    while (temp != NULL) {
        n++;
        temp = temp->next;
    }
    *array = malloc(sizeof(int16_t) + (n * 2 * sizeof(int16_t)));
    (*array)[0] = -2;
    int i = 0; 
    temp = list;
    while (temp != NULL && i < n) {
        (*array)[1 + (i * 2)] = ((struct Bullet*) temp->data)->position.x;
        (*array)[2 + (i * 2)] = ((struct Bullet*) temp->data)->position.y;
        i++;
        temp = temp->next;
    }
    return n;
}

void* server_send_loop(void *arg) {
    int socket = *((int *) arg);
    int16_t tab[3];
    struct timeval start, stop;
    double time_interval;
    int killer;
    while (1) {
        gettimeofday(&start, NULL);
        int i, j;
        move_bullets(&bullets_server);
        for (i = 0; i < number_of_connected_clients; i++) {
            move_player(&players_server[i]);
            if (check_if_player_dies(&players_server[i], &bullets_server, &killer)) {
                players_server[i].position.x = SPAWN_X;
                players_server[i].position.y = SPAWN_Y;
                players_server[i].deaths++;
                players_server[killer].kills++;
            }
        }
        int16_t *bullet_array = NULL;
        int bullets_n = get_bullet_array(bullets_server, &bullet_array);
        for (i = 0; i < number_of_connected_clients; i++) {
            for (j = 0; j < number_of_connected_clients; j++) {
                tab[0] = j;
                tab[1] = players_server[j].position.x;
                tab[2] = players_server[j].position.y;
                tab[3] = players_server[j].kills;
                tab[4] = players_server[j].deaths;
                send_data(socket, clients_addresses[i], tab, 5);
                usleep(20);
            }
            send_data(socket, clients_addresses[i], bullet_array, 1 + (bullets_n * 2));
            usleep(20);
        }
        free(bullet_array);
        gettimeofday(&stop, NULL);
        time_interval = (double) (stop.tv_usec - start.tv_usec);
        if ((double) (stop.tv_usec - start.tv_usec) > 0) {
            time_interval = (double) (stop.tv_usec - start.tv_usec);
        }
        usleep(FRAME_TIME - time_interval);
    }
}


int its_an_old_client(int client_pos) {
    return (client_pos < number_of_connected_clients && client_pos >= 0);
}

void add_adr_to_list(int client_pos, struct sockaddr_in *client_addr) {
    if (client_pos >= number_of_connected_clients) {
        clients_addresses[number_of_connected_clients++] = *client_addr;
    }
}
