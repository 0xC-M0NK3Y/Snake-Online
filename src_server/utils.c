#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

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