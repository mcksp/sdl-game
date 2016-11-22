#include "client_udp.h"


void prepare_client(int *sock, struct sockaddr_in *client_addr) {
    if ((*sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("client socket failed");
    }

    if (bind(*sock, (struct sockaddr*)client_addr, sizeof(struct sockaddr)) < 0) {
        perror("bind client error");
    }
}

void send_to_server(int sock, struct sockaddr_in serv_addr, int16_t id, float x, float y) {
    int16_t tab[3];
    tab[0] = id;
    tab[1] = (int16_t) x;
    tab[2] = (int16_t) y;
    socklen_t serv_addr_size = sizeof(struct sockaddr);
    if (sendto(sock, tab, sizeof(int16_t) * 3 + 1, 0,
            (struct sockaddr *) &serv_addr, serv_addr_size) < 0) {
        perror("sendto error");
    }
 
}

void client_listen(int sock, int *id, int *x, int *y){
    int16_t tab[3];
    int length = recvfrom(sock, tab, sizeof(int16_t)*3 + 1, 0, NULL, 0);
    if (length > 0) {
        *id = tab[0];
        *x = tab[1];
        *y = tab[2];
    }
}
