#ifndef STRUCTS_H
# define STRUCTS_H

#include "../config.h"

#include "defines.h"

typedef struct pos {
    int x;
    int y;
}   pos_t;

typedef struct buffer {
    uint8_t *ptr;
    size_t  len;
}   buffer_t;

typedef struct client {
    char        map[MAP_HEIGHT][MAP_WIDTH];
    char        username[16];
    socket_t    sock;
    buffer_t    buffer;
    int         state;
    pos_t       player;
    int         dir;
    int         len;
    pos_t       snake[MAX_SNAKE_LENGHT];
}   client_t;

typedef struct map_env {
    pos_t   apples[MAX_APPLE];
}   map_env_t;

#endif /* structs.h */