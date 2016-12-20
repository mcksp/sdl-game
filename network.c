#include "network.h"
#define TRUE 1
#define FALSE 0

struct sockaddr_in server_sock_addr() {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(0x7f000001);
    serv_addr.sin_port = htons(1234);
    return serv_addr;
}

struct sockaddr_in client_sock_addr() {
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_ANY;
    client_addr.sin_port = 0;
    return client_addr;
}


int addr_pos_in_tab(struct sockaddr_in new_addr, struct sockaddr_in old_addr_tab[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        if(compare_addr(&new_addr, &old_addr_tab[i])) {
            return i;
        }
    }
    return size;
}

int compare_addr(struct sockaddr_in *a, struct sockaddr_in *b) {
    if (a->sin_port == b->sin_port &&
            a->sin_family == b->sin_family &&
            a->sin_addr.s_addr == b->sin_addr.s_addr) {
        return 1;
    } else {
        return 0;
    }
}

int16_t key_state_from_player(struct Player *player) {
    int16_t key_state = 0;
    if (player->left_key) {
        key_state = key_state | 0x01;
    }
    if (player->right_key) {
        key_state = key_state | 0x02;
    }
    if (player->up_key) {
        key_state = key_state | 0x04;
    }
    if (player->down_key) {
        key_state = key_state | 0x08;
    }
    if (player->attack_key) {
        key_state = key_state | 0x10;
    }
    return key_state;
}

void player_from_key_state(struct Player *player, int16_t key_state) {
    if (key_state & 0x01) {
        player->left_key = TRUE;
    } else {
        player->left_key = FALSE;
    }
    if (key_state & 0x02) {
        player->right_key = TRUE;
    } else {
        player->right_key = FALSE;
    }
    if (key_state & 0x04) {
        player->up_key = TRUE;
    } else {
        player->up_key = FALSE;
    }
    if (key_state & 0x08) {
        player->down_key = TRUE;
    } else {
        player->down_key = FALSE;
    }
    if (key_state & 0x10) {
        player->attack_key = TRUE;
    } else {
        player->attack_key = FALSE;
    }
}
