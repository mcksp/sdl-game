#ifndef SERVER_DUP_H
#define SERVER_DUP_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdint.h>

#define SERVER_PORT 1234

void prepare_server(int *sock, struct sockaddr_in *server_sock);
void receive_data(int sock);

#endif
