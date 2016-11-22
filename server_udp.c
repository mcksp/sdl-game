#include "server_udp.h"

void prepare_server(int *sock, struct sockaddr_in *server_sock) {
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
    int length = recvfrom(sock, data, 10, 0, (struct sockaddr*)&addr, &addr_size);
    return addr;
}

void send_data(int sock, struct sockaddr_in client, int16_t tab[]) {
    sendto(sock, tab, 3 * sizeof(int16_t)+1, 0, (struct sockaddr*)&client, sizeof(struct sockaddr));
}


