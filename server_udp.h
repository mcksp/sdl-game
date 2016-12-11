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
void send_data(int sock, struct sockaddr_in client, int16_t tab[], int size);
void* server_receive_loop(void *arg);
void* server_send_loop(void *arg);
int its_an_old_client(int client_pos);
void add_adr_to_list(int client_pos, struct sockaddr_in *client_addr);

#endif
