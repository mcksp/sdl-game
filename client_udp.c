#include "client_udp.h"


void prepare_client(int *sock, struct sockaddr_in *client_addr) {
    if ((*sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("client socket failed");
    }

    if (bind(*sock, (struct sockaddr*)client_addr, sizeof(struct sockaddr)) < 0) {
        perror("bind client error");
    }
}

void send_to_server(int sock, struct sockaddr_in serv_addr, float x, float y) {
    int32_t tab[2];
    tab[0] = (int32_t) x;
    tab[1] = (int32_t) y;
    socklen_t serv_addr_size = sizeof(struct sockaddr);
    if (sendto(sock, tab, sizeof(int32_t) * 2 + 1, 0,
            (struct sockaddr *) &serv_addr, serv_addr_size) < 0) {
        perror("sendto error");
    }
 
}

void client_listen(int sock){
    char tab[10];
    int length = recvfrom(sock, tab, 9, 0, NULL, 0);
    if (length > 0) {
        printf("client: %s\n", tab);
    }
}
