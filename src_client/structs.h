#ifndef STRUCTS_H
# define STRUCTS_H

#include <netinet/in.h>

#include <poll.h>

#include <SDL2/SDL.h>

#include "defines.h"

// environement sdl
typedef struct sdlenv {
    SDL_Window      *window;     // fenetre
    SDL_Renderer    *renderer;   // rendrerer
    SDL_Event       event;       // evenements
    flag_t          window_flag; // flag de la fenetre
}   sdlenv_t;

// parametre de la fenetre
typedef struct param {
    char username[16];
    char winner[16];
    int state;
    int running;
    int fullscreen;
    int width;
    int height;
}   param_t;

typedef struct map {
    char        ptr[MAP_HEIGHT][MAP_WIDTH];
    int         pos_x;
    int         pos_y;
    int         dir;
    int         len;
}   map_t;

typedef struct conn_manager {
    socket_t        sock;
    sockaddr_in_t   addr;
    port_t          port;
    pollfd_t        pollfd;
    int             ok;
    uint8_t         *buffer;
    int             buffer_len;
}   conn_manager_t;

typedef struct gameenv {
    sdlenv_t        sdl;
    param_t         param;
    map_t           map;
    conn_manager_t  conn;
}   gameenv_t;

#endif /* structs.h */