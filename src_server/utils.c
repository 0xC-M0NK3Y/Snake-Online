#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <stddef.h>

#include <unistd.h>

#include "structs.h"

int to_parse(uint8_t *buffer, int buffer_size) {
    if (buffer_size < 7)
        return 0;
    return ((int)*(uint32_t *)buffer <= buffer_size)?1:0;
}

void free_buffers(client_t *clients, size_t clients_len) {
    for (size_t i = 1; i < clients_len; i++) {
        free(clients[i].buffer.ptr);
    }
}

int send_finish_packets(client_t *clients, size_t clients_len, char *winner) {
    uint8_t packet[MAX_PACKET_LEN] = {0};
    int i = 0;

    *(uint32_t *)packet = sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint16_t) + strlen(winner);
    i += sizeof(uint32_t);
    *(uint16_t *)&packet[i] = FINISH_PACKET_ID;
    i += sizeof(uint16_t);
    *(uint16_t *)&packet[i] = strlen(winner);
    i += sizeof(uint16_t);
    memcpy(&packet[i], winner, strlen(winner));
    i += strlen(winner);
    for (size_t j = 0; j < clients_len; j++) {
        if (write(clients[j].sock, packet, i) != i)
            printf("Failed send win packet to %s\n", clients[j].username);
    }
    return 1;
}