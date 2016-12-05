#include "server_udp.h"
#include "network.h"
#include "objects.h"

#define MAX_PLAYERS 10

struct sockaddr_in clients_addresses[MAX_PLAYERS];
struct Player players_server[MAX_PLAYERS];
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
    recvfrom(sock, data, sizeof(int16_t) * 3 + 1, 0, (struct sockaddr*)&addr, &addr_size);
    return addr;
}

void send_data(int sock, struct sockaddr_in client, int16_t data[]) {
    socklen_t addr_size = sizeof(struct sockaddr);
    sendto(sock, data, sizeof(int16_t) * 3 + 1, 0, (struct sockaddr*)&client, addr_size);
}


void* server_receive_loop(void *arg) {
    int socket = *((int *) arg);
    int client_pos = 0;
    struct sockaddr_in client_addr;
    int16_t tab[3];

    while (1) {
        client_addr = receive_data(socket, tab);
        client_pos = addr_pos_in_tab(client_addr, clients_addresses, number_of_connected_clients);
        if (its_an_old_client(client_pos)) {
            players_server[client_pos].position.x = tab[1];
            players_server[client_pos].position.y = tab[2];
        }
        if (tab[0] == -1 && client_pos < MAX_PLAYERS) {
            add_adr_to_list(client_pos, &client_addr);
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
                    tab[1] = players_server[j].position.x;
                    tab[2] = players_server[j].position.y;
                    send_data(socket, clients_addresses[i], tab);
                }
            }
        }
        usleep(50);
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
