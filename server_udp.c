#include "server_udp.h"
#include "network.h"
#include "objects.h"
#include "list.h"
#include "physic.h"

#define MAX_PLAYERS 10

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


void* server_receive_loop(void *arg) {
    int socket = *((int *) arg);
    int client_pos = 0;
    struct sockaddr_in client_addr;
    int16_t tab[4];

    while (1) {
        client_addr = receive_data(socket, tab);
        client_pos = addr_pos_in_tab(client_addr, clients_addresses, number_of_connected_clients);
        if (its_an_old_client(client_pos)) {
            players_server[client_pos].position.x = tab[1];
            players_server[client_pos].position.y = tab[2];
            if (players_server[client_pos].shoot == 0 && tab[3] != 0) {
                struct Bullet temp;
                temp.position.x = tab[1];
                temp.position.y = tab[2];
                temp.position.w = 8;
                temp.position.h = 8;
                temp.face = tab[3];
                push_element(&bullets_server, &temp, sizeof(struct Bullet));
            }
            players_server[client_pos].shoot = tab[3]; 
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
    while (1) {
        int i, j;
        move_bullets(&bullets_server);
        int16_t *bullet_array = NULL;
        int bullets_n = get_bullet_array(bullets_server, &bullet_array);
        for (i = 0; i < number_of_connected_clients; i++) {
            for (j = 0; j < number_of_connected_clients; j++) {
                if (i != j) {
                    tab[0] = j;
                    tab[1] = players_server[j].position.x;
                    tab[2] = players_server[j].position.y;
                    send_data(socket, clients_addresses[i], tab, 3);
                }
                usleep(200);
            }
            send_data(socket, clients_addresses[i], bullet_array, 1 + (bullets_n * 2));
            usleep(200);
        }
        free(bullet_array);
        usleep(10000);
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
