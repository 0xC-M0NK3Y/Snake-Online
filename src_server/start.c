#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "structs.h"

static int generate_start_positions(client_t *clients, size_t clients_len, map_env_t *mapenv) {

    /* Do recursion until we have a valid map */

    /* Generate apples pos, if 2 on same spot is ok */
    for (int i = 0; i < MAX_APPLE; i++) {
        mapenv->apples[i].y = rand() % MAP_HEIGHT;
        mapenv->apples[i].x = rand() % MAP_WIDTH;
    }
    /* memset all clients map to the background */
    for (size_t i = 1; i < clients_len; i++) {
        for (int y = 0; y < MAP_HEIGHT; y++)
            memset(clients[i].map[y], BACKGROUND, MAP_WIDTH);
    }
    /* generate randome start positions and dir */
    for (size_t i = 1; i < clients_len; i++) {
        clients[i].player.x = rand() % MAP_WIDTH;
        clients[i].player.y = rand() % MAP_HEIGHT;
        /* verify if not spawn on apple */
        for (int j = 0; j < MAX_APPLE; j++) {
            if (clients[i].player.y == mapenv->apples[j].y && clients[i].player.x == mapenv->apples[j].x)
                return generate_start_positions(clients, clients_len, mapenv);
            clients[i].map[mapenv->apples[j].y][mapenv->apples[j].x] = APPLE;
        }
        /* setup the snake */
        clients[i].snake[0].y = clients[i].player.y;
        clients[i].snake[0].x = clients[i].player.y;
        clients[i].dir = rand() % 4;
        clients[i].len = 1;
        clients[i].map[clients[i].player.y][clients[i].player.x] = SNAKE;
        /* setup it up for the other clients */
        for (size_t j = 1; j < clients_len; j++) {
            if (i != j)
                clients[j].map[clients[i].player.y][clients[i].player.x] = ENNEMY;
        }
    }
    /* verify if two snake don't spawn on the same spot */
    for (size_t i = 1; i < clients_len; i++) {
        for (size_t j = 1; j < clients_len; j++) {
            if (j != i) {
                if (clients[i].player.x == clients[j].player.x && clients[i].player.y == clients[j].player.y)
                    return generate_start_positions(clients, clients_len, mapenv);
            }
        }
    }
    return 1;
}

#include <stdio.h>
int send_start_packets(client_t *clients, size_t client_len, map_env_t *mapenv) {
    uint8_t packet[MAX_PACKET_LEN] = {0};
    int tmp;
    int i = 0;

    *(uint32_t *)packet = sizeof(uint32_t) + sizeof(uint16_t) + 1 + (4*sizeof(int));
    i += sizeof(uint32_t);
    *(uint16_t *)&packet[i] = START_PACKET_ID;
    i += sizeof(uint16_t);
    packet[i] = 'S';
    i++;
    tmp = i;
    generate_start_positions(clients, client_len, mapenv);
    for (size_t j = 1; j < client_len; j++) {
        i = tmp;
        *(int *)&packet[i] = clients[j].player.x;
        i += sizeof(int);
        *(int *)&packet[i] = clients[j].player.y;
        i += sizeof(int);
        *(int *)&packet[i] = clients[j].dir;
        i += sizeof(int);
        *(int *)&packet[i] = clients[j].len;
        i += sizeof(int);
        if (write(clients[j].sock, packet, i) != i) {
            printf("Failed send start packet to %s\n", clients[j].username);
        }
    }
    return 1;
}