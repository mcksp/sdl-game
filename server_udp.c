#include "server_udp.h"


void prepare_server(int *sock, struct sockaddr_in *server_sock) {
    if ((*sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket failed");
    }
    if (bind(*sock, (struct sockaddr*) server_sock, sizeof(struct sockaddr)) < 0) {
        perror("bind server error");
    }
}

void receive_data(int sock) {
    char tab[10];
    int length = recvfrom(sock, tab, 11, 0, NULL, NULL);
    if (length > 0) {
        printf("length: %d\n", length);
        printf("%d\n", tab[0]);
    }
    
}

