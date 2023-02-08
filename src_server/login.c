#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include "structs.h"

static int send_confirm_packet(client_t *client, uint32_t packet_len) {
    uint8_t packet[MAX_PACKET_LEN] = {0};
    int i = 0;

    *(uint32_t *)packet = packet_len;
    i += sizeof(uint32_t);
    *(uint16_t *)&packet[i] = CONFIRM_PACKET_ID;
    i += sizeof(uint16_t);
    *(uint16_t *)&packet[i] = strlen(client->username);
    i += sizeof(uint16_t);
    memcpy(&packet[i], client->username, strlen(client->username));
    i += strlen(client->username);
    if (write(client->sock, packet, i) != i) {
        printf("Failed send confirm packet\n");
        return -1;
    }
    return 1;
}

#include <stdio.h>
int handle_login(client_t *client) {
    uint8_t *buffer = client->buffer.ptr;
    uint16_t string_len;
    uint32_t packet_len;
    uint16_t packet_id;
    int i = 0;

    packet_len = *(uint32_t *)buffer;
    i += sizeof(uint32_t);
    packet_id = *(uint16_t *)&buffer[i];
    i += sizeof(uint16_t);
    if (packet_id != LOGIN_PACKET_ID)
        return -1;
    string_len = *(uint16_t *)&buffer[i];
    i += sizeof(uint16_t);
    if (string_len >= 16)
        return -1;
    memcpy(client->username, &buffer[i], string_len);
    i += string_len;
    client->buffer.len -= i;
    memmove(buffer, &buffer[i], client->buffer.len);
    printf("Received login packet from %s\n", client->username);
    send_confirm_packet(client, packet_len);
    client->state = WAITING;
    return 1;
}