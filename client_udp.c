#include "client_udp.h"

int fd;
struct sockaddr_in serveraddr;

void prepare_client() {
    if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket failed");
    }

    memset( &serveraddr, 0, sizeof(serveraddr) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVER_PORT);              
    serveraddr.sin_addr.s_addr = htonl( 0x7f000001 );  
}

void send_to_server(float x, float y) {
    int32_t tab[2];
    tab[0] = (int32_t) x;
    tab[1] = (int32_t) y;
    sendto(fd,tab, sizeof(int32_t) * 2, 0,
            (struct sockaddr *) &serveraddr, sizeof(serveraddr));
}

