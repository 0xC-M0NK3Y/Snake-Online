#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <poll.h>

#include <time.h>

#include "../config.h"

#include "structs.h"
#include "utils.h"
#include "clients.h"
#include "login.h"
#include "start.h"
#include "play.h"

int main(int argc, char **argv)
{
    pollfd_t        pollfd[PLAYER_NEEDED+1] = {0};
    client_t        clients[PLAYER_NEEDED+1] = {0};
    map_env_t       mapenv = {0};
    socket_t        sock;
    size_t          clients_len = 0;
    sockaddr_in_t   addr = {0};
    int             started = 0;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(atoi(argv[1]));

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return perror("socket"), EXIT_FAILURE;
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return perror("Binding"), 1; 
    if (listen(sock, 0) < 0)
        return perror("Listening"), 1;

    pollfd[0].fd = sock;
    pollfd[0].events = POLLIN;
    pollfd[0].revents = 0;

    clients_len = 1;

    printf("Server listenning\n\n");
    while (1)
    {
        if (poll(pollfd, clients_len, 100) < 0)
            return perror("Poll"), EXIT_FAILURE;
        for (size_t i = 0; i < clients_len; i++) {
            if (pollfd[i].fd <= 0)
                continue;
            if ((pollfd[i].revents & POLLIN) == POLLIN) {
                if (pollfd[i].fd == sock) {
                    socket_t        csock;
                    socklen_t       clenght;
                    sockaddr_in_t   caddr = {0};

                    csock = accept(sock, (struct sockaddr *)&caddr, &clenght);
                    if (csock < 0) {
                        printf("Failed accpet a client\n");
                        continue;
                    }
                    pollfd[clients_len].fd = csock;
                    pollfd[clients_len].events = POLLIN;
                    pollfd[clients_len].revents = 0;

                    clients[clients_len].buffer.ptr = NULL;
                    clients[clients_len].buffer.len = 0;
                    clients[clients_len].sock = csock;
                    clients[clients_len].state = LOGGING;
                    clients_len++;
                } else {
                    if (client_recv(&clients[i]) < 0) {
                            printf("Disconnection\n");
                            close(clients[i].sock);
                            free_buffers(clients, clients_len);
                            exit(1);
                    }
                    while (to_parse(clients[i].buffer.ptr, clients[i].buffer.len))
                    {
                        switch (clients[i].state) {
                        case LOGGING: if (handle_login(&clients[i]) < 0) printf("Probleme with a client"); break;
                        case PLAYING: if (handle_play(&clients[i]) < 0) printf("bad play packet\n"); break;
                        default: break;
                        }
                    }
                }
            }
        }
        if (started) {
            move_players(clients, clients_len, &mapenv);
            send_play_packets(clients, clients_len);
        }
        if (!started && clients_len > PLAYER_NEEDED) {
            size_t test = 0;
            for (size_t i = 1; i < clients_len; i++) {
                if (clients[i].state == WAITING)
                    test++;
            }
            if (test != clients_len - 1)
                continue;
            printf("Game start\n");
            send_start_packets(clients, clients_len, &mapenv);
            send_play_packets(clients, clients_len);
            started = 1;
            for (size_t i = 1; i < clients_len; i++)
                clients[i].state = PLAYING;
        }
    }
}