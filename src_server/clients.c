#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "structs.h"

int client_recv(client_t *client) {
    char    buffer[MAX_PACKET_LEN] = {0};
    long    r;

    r = read(client->sock, buffer, MAX_PACKET_LEN);
    if (r == 0)
        return -1;
    /* If already 5 minimum, or more packets to parse ignore this one to avoid spam and too much realloc */
    if (client->buffer.len > 5*MAX_PACKET_LEN)
        return 1;
    client->buffer.ptr = realloc(client->buffer.ptr, r + client->buffer.len);
    if (client->buffer.ptr == NULL)
        return -1;
    memcpy(&client->buffer.ptr[client->buffer.len], buffer, r);
    client->buffer.len += r;
    return 1;
}