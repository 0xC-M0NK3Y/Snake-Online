#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include "../config.h"

#include "defines.h"
#include "structs.h"

static int handle_finish_packet(uint8_t *buffer, int *buffer_len, int i, param_t *param, conn_manager_t *conn) {
    uint16_t winner_len;

    winner_len = *(uint16_t *)&buffer[i];
    i += sizeof(uint16_t);
    if (winner_len >= 16)
        return -1;
    memcpy(param->winner, &buffer[i], winner_len);
    i += winner_len;
    buffer_len[0] -= i;
    memmove(buffer, &buffer[i], buffer_len[0]);
    param->state = FINISHED;
    conn->ok = 0;
    return 1;
}

int handle_play(uint8_t *buffer, int *buffer_len, map_t *map, param_t *param, conn_manager_t *conn) {
    uint32_t packet_len;
    uint16_t packet_id;
    int      i = 0;

    packet_len = *(uint32_t *)buffer;
    i += sizeof(uint32_t);
    packet_id = *(uint16_t *)&buffer[i];
    i += sizeof(uint16_t);
    if (packet_id != PLAY_PACKET_ID && packet_id != FINISH_PACKET_ID) {
        printf("packet length = %d and id = %d\n", packet_len, packet_id);
        return -1;
    }
    if (packet_id == FINISH_PACKET_ID)
        return handle_finish_packet(buffer, buffer_len, i, param, conn);
    memcpy(map->ptr, &buffer[i], MAP_HEIGHT * MAP_WIDTH);
    i += MAP_HEIGHT * MAP_WIDTH;
    map->pos_x = *(int *)&buffer[i];
    i += sizeof(int);
    map->pos_y = *(int *)&buffer[i];
    i += sizeof(int);
    map->dir = *(int *)&buffer[i];
    i += sizeof(int);
    map->len = *(int *)&buffer[i];
    i += sizeof(int);
    buffer_len[0] -= i;
    memmove(buffer, &buffer[i], buffer_len[0]);
    return 1;
}

static int send_play_packet(int dir, socket_t sock) {
    uint8_t packet[MAX_PACKET_LEN] = {0};
    int i = 0;

    *(uint32_t *)packet = sizeof(uint32_t) + sizeof(uint16_t) + sizeof(int);
    i += sizeof(uint32_t);
    *(uint16_t *)&packet[i] = PLAY_PACKET_ID;
    i += sizeof(uint16_t);
    *(int *)&packet[i] = dir;
    i += sizeof(int);
    if (write(sock, packet, i) != i) {
        printf("Failed send play placket\n");
        return -1;
    }
    return 1;
}

int change_direction(const map_t *map, int dir, socket_t sock) {
    if (map->dir == dir)
        return -1;
    if (map->dir == UP && dir == DOWN)
        return -1;
    if (map->dir == DOWN && dir == UP)
        return -1;
    if (map->dir == LEFT && dir == RIGHT)
        return -1;
    if (map->dir == RIGHT && dir == LEFT)
        return -1;
    if (send_play_packet(dir, sock) < 0)
        return -1;
    return 1;
}