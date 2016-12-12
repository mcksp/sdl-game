#include "client_udp.h"
#define BUF_MAX 256

void prepare_client(int *sock, struct sockaddr_in *client_addr) {
    if ((*sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("client socket failed");
    }

    if (bind(*sock, (struct sockaddr*)client_addr, sizeof(struct sockaddr)) < 0) {
        perror("bind client error");
    }
}

void send_to_server(int sock, struct sockaddr_in serv_addr, int16_t id, float x, float y, int16_t shoot) {
    int16_t tab[4];
    tab[0] = id;
    tab[1] = (int16_t) x;
    tab[2] = (int16_t) y;
    tab[3] = shoot;
    socklen_t serv_addr_size = sizeof(struct sockaddr);
    if (sendto(sock, tab, sizeof(int16_t) * 4, 0,
            (struct sockaddr *) &serv_addr, serv_addr_size) < 0) {
        perror("sendto error");
    }
 
}

void client_listen(int sock, int *id, int *x, int *y, int16_t *bullet_array){
    int16_t tab[BUF_MAX];
    int length = recvfrom(sock, tab, sizeof(int16_t) * BUF_MAX, 0, NULL, 0);
    if (length > 0) {
        *id = tab[0];
        if (tab[0] != -2) {
            *x = tab[1];
            *y = tab[2];
        } else {
            memcpy(bullet_array, tab + 1, length - sizeof(int16_t));
            *x = (length - sizeof(int16_t)) / (sizeof(int16_t) * 2);
        }
    }
}
