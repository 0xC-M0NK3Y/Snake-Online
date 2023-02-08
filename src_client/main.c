#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <poll.h>

#include <SDL2/SDL.h>

#include "../config.h"

#include "inits.h"
#include "structs.h"
#include "utils.h"
#include "login.h"
#include "play.h"
#include "display.h"

int main(int argc, char **argv)
{
    gameenv_t   env = {0};
    param_t     *param = &env.param;
    SDL_Event   *event = &env.sdl.event;

    (void)env;
    if (argc != 4) {
        printf("Usage: %s <ip> <port> <username>\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (init_game(&env, argv[1], argv[2], argv[3]) < 0) {
        printf("CRITICAL ERROR\n");
        return EXIT_FAILURE;
    }
    printf("IN MAIN %p\n", &env.map.pos_x);
    while (param->running)
    {
        if (poll(env.conn.pollfd, 1, -1) < 0)
            return perror("Poll"), 1;
        for (int i = 0; i < 1; i++) {
            if (env.conn.pollfd[i].revents & POLLIN) {
                if (i == 0) {
                    uint8_t buffer[MAX_PACKET_LEN] = {0};
                    int r = read(env.conn.sock, buffer, MAX_PACKET_LEN);

                    if (r == 0)
                        return EXIT_FAILURE;
                    env.conn.buffer = realloc(env.conn.buffer, r + env.conn.buffer_len);
                    if (env.conn.buffer == NULL)
                        return EXIT_FAILURE;
                    memcpy(&env.conn.buffer[env.conn.buffer_len], buffer, r);
                    env.conn.buffer_len += r;
                }
            }
        }
        while (to_parse(env.conn.buffer, env.conn.buffer_len))
        {
            switch (env.param.state) {
            case LOGGING: if (handle_logging(env.conn.buffer, &env.conn.buffer_len, env.param.username) < 0) return EXIT_FAILURE; env.param.state = WAITING; break;
            case WAITING: if (handle_waiting(env.conn.buffer, &env.conn.buffer_len, &env.param.state, &env.map) < 0) return EXIT_FAILURE; break;
            case PLAYING: if (handle_play(env.conn.buffer, &env.conn.buffer_len, &env.map) < 0) {puts("bad play packet"); return EXIT_FAILURE;} break;
            default: break;
            }
        }
        /* Listenning to any events */
        while (SDL_PollEvent(event))
        {
            /* If keydown */
            if (event->type == SDL_KEYDOWN) {
                switch (event->key.keysym.sym) {
                    /* Escape key to quit */
                    case SDLK_ESCAPE: param->running = FALSE; break;
                    case SDLK_q: change_direction(&env.map, LEFT, env.conn.sock); break;
                    case SDLK_z: change_direction(&env.map, UP, env.conn.sock); break;
                    case SDLK_d: change_direction(&env.map, RIGHT, env.conn.sock); break;
                    case SDLK_s: change_direction(&env.map, DOWN, env.conn.sock); break;
                    default: break;
                }
            }
            /* If quit, not relevant in fullscreen */
            if (event->type == SDL_QUIT)
                param->running = FALSE;
        }
        if (env.param.state == PLAYING)
            display_map(env.sdl.renderer, &env.map);
    }
    free(env.conn.buffer);
    close(env.conn.sock);
    SDL_DestroyWindow(env.sdl.window);
    SDL_Quit();
    return EXIT_SUCCESS;
}