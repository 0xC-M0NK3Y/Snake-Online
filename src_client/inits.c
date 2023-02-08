#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <poll.h>

#include <SDL2/SDL.h>

#include "../config.h"

#include "structs.h"
#include "login.h"

static int init_sdl(sdlenv_t *sdl) {
    /* SDL init */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return -__LINE__;
    /* flag of the window */
    sdl->window_flag = SDL_WINDOW_SHOWN; //| SDL_WINDOW_BORDERLESS;
    /* create the window */
    sdl->window = SDL_CreateWindow("Snake-Online", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, sdl->window_flag);
    if (sdl->window == NULL)
        return -__LINE__;
    /* get the main surface */
    sdl->renderer = SDL_CreateRenderer(sdl->window, -1, 0);
    if (sdl->renderer == NULL)
        return -__LINE__;
    /* fill the background */
    SDL_SetRenderDrawColor(sdl->renderer, 10, 10, 10, 255);
    SDL_RenderClear(sdl->renderer);
    SDL_RenderPresent(sdl->renderer);
    return 1;
}

static int init_conn(conn_manager_t *conn, char *ip, char *port) {
    conn->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (conn->sock < 0)
        return -__LINE__;
    conn->port = htons(atoi(port));
    conn->addr.sin_addr.s_addr = inet_addr(ip);
    conn->addr.sin_family = AF_INET;
    conn->addr.sin_port = conn->port;

    conn->pollfd[0].fd = conn->sock;
    conn->pollfd[0].events = POLLIN;
    conn->pollfd[0].revents = 0;

    if (connect(conn->sock, (struct sockaddr *)&conn->addr, sizeof(sockaddr_in_t)) < 0)
        return perror("Connect"), -1;
    return 1;
}

int init_game(gameenv_t *env, char *ip, char *port, char *username) {

    int r;

    if (strlen(username) >= 16) {
        printf("Username to long\n");
        return -1;
    }
    memcpy(env->param.username, username, strlen(username));
    /* Init connexion to server */
    r = init_conn(&env->conn, ip, port);
    if (r < 0)
        return r;
    if (send_login_packet(env->conn.sock, username) < 0)
        return -__LINE__;
    /* Init SDL */
    r = init_sdl(&env->sdl);
    if (r < 0)
        return r;
    /* Set up parameters of the game, window */
    env->param.running = TRUE;
    env->param.fullscreen = TRUE;
    SDL_GetWindowSize(env->sdl.window, &env->param.width, &env->param.height);
    env->param.state = LOGGING;
    return 1;
}