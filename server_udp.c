#include "server_udp.h"

int serv_sock;
struct sockaddr_in myaddr;

void prepare_server() {
    memset(&myaddr, 0, sizeof(struct sockaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(SERVER_PORT);

    serv_sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (serv_sock < 0) {
        perror("socket failed");
    }

    bind(serv_sock, (struct sockaddr*)&myaddr, sizeof(struct sockaddr));
}

void receive_data() {
    int32_t tab[2];
    int length = recvfrom(serv_sock, tab, sizeof(int32_t) * 2, 0, NULL, 0);
    if (length > 0) {
        printf("length: %d\n", length);
        printf("%d\n", tab[0]);
    }
}


void close_server() {
    close(serv_sock);
}
