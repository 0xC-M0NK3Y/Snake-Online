#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include "../config.h"

#include "structs.h"

int handle_logging(uint8_t *buffer, int *buffer_len, char *verif) {
    uint32_t packet_len;
    uint16_t packet_id;
    uint16_t string_len;
    int      i = 0;

    packet_len = *(uint32_t *)buffer;
    i += sizeof(uint32_t);
    packet_id = *(uint16_t *)&buffer[i];
    i += sizeof(uint16_t);
    if (packet_id != CONFIRM_PACKET_ID)
        return -1;
    string_len = *(uint16_t *)&buffer[i];
    i += sizeof(uint16_t);
    if (strncmp(verif, (const char *)&buffer[i], string_len) != 0)
        return -1;
    i += string_len;
    buffer_len[0] -= packet_len;
    memmove(buffer, &buffer[i], buffer_len[0]);
    return 1;
}

int handle_waiting(uint8_t *buffer, int *buffer_len, int *state, map_t *map) {
    uint32_t packet_len;
    uint16_t packet_id;
    int      i = 0;

    packet_len = *(uint32_t *)buffer;
    i += sizeof(uint32_t);
    packet_id = *(uint16_t *)&buffer[i];
    i += sizeof(uint16_t);
    if (packet_id != START_PACKET_ID)
        return -1;
    if ((char)buffer[i] != 'S')
        return -1;
    i++;
    map->pos_x = *((int *)&buffer[i]);
    i += sizeof(int);
    map->pos_y = *(int *)&buffer[i];
    i += sizeof(int);
    map->dir = *(int *)&buffer[i];
    i += sizeof(int);
    map->len = *(int *)&buffer[i];
    i += sizeof(int);
    *state = PLAYING;
    if ((uint32_t)i != packet_len) {
        printf("Failed parse good\n");
        return -1;
    }
    buffer_len[0] -= i;
    memmove(buffer, &buffer[i], buffer_len[0]);
    printf("Game starting\n");
    return 1;
}

int send_login_packet(socket_t sock, char *username) {
    uint8_t packet[MAX_PACKET_LEN] = {0};
    uint32_t i = 4;

    *(uint16_t *)&packet[i] = LOGIN_PACKET_ID;
    i += 2;
    *(uint16_t *)&packet[i] = strlen(username);
    i += 2;
    memcpy(&packet[i], username, strlen(username));
    i += strlen(username);
    *(uint32_t *)packet = i;
    if (write(sock, packet, i) != i) {
        printf("Failed send login packet\n");
        return -1;
    }
    return 1;
}
