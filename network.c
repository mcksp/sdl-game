#include "network.h"
#include "constans.h"

struct sockaddr_in server_sock_addr(char *ip) {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if (ip == NULL) {
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        inet_aton(ip, &serv_addr.sin_addr);
    }
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
        return true;
    } else {
        return false;
    }
}

int16_t key_state_from_player(struct Player *player) {
    int16_t key_state = 0;
    if (player->left) {
        key_state = key_state | LEFT_KEY;
    }
    if (player->right) {
        key_state = key_state | RIGHT_KEY;
    }
    if (player->up) {
        key_state = key_state | UP_KEY;
    }
    if (player->down) {
        key_state = key_state | DOWN_KEY;
    }
    if (player->shoot) {
        key_state = key_state | ATTACK_KEY;
    }
    return key_state;
}

void player_from_key_state(struct Player *player, int16_t key_state) {
    if (key_state & LEFT_KEY) {
        player->left = true;
    } else {
        player->left = false;
    }
    if (key_state & RIGHT_KEY) {
        player->right = true;
    } else {
        player->right = false;
    }
    if (key_state & UP_KEY) {
        player->up = true;
    } else {
        player->up = false;
    }
    if (key_state & DOWN_KEY) {
        player->down = true;
    } else {
        player->down = false;
    }
    if (key_state & ATTACK_KEY) {
        player->shoot = true;
    } else {
        player->shoot = false;
    }
}
