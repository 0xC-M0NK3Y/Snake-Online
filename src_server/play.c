#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "structs.h"
#include "utils.h"

int send_play_packets(client_t *clients, size_t clients_len) {
    uint8_t packet[MAX_PACKET_LEN] = {0};
    int tmp;
    int i = 0;

    *(uint32_t *)packet = sizeof(uint32_t) + sizeof(uint16_t) + MAP_HEIGHT * MAP_WIDTH + (sizeof(int) * 4);
    i += sizeof(uint32_t);
    *(uint16_t *)&packet[i] = PLAY_PACKET_ID;
    i += sizeof(uint16_t);
    tmp = i;
    for (size_t j = 1; j < clients_len; j++) {
        i = tmp;
        memcpy(&packet[i], clients[j].map, MAP_HEIGHT * MAP_WIDTH);
        i += MAP_HEIGHT * MAP_WIDTH;
        *(int *)&packet[i] = clients[j].player.x;
        i += sizeof(int);
        *(int *)&packet[i] = clients[j].player.y;
        i += sizeof(int);
        *(int *)&packet[i] = clients[j].dir;
        i += sizeof(int);
        *(int *)&packet[i] = clients[j].len;
        i += sizeof(int);
        if (write(clients[j].sock, packet, i) != i) {
            printf("Failed send play packet to %s\n", clients[j].username);
        }
    }
    return 1;
}

int move_players(client_t *clients, size_t clients_len, map_env_t *mapenv) {

    /* reset map */
    for (size_t i = 1; i < clients_len; i++) {
        memset(clients[i].map, BACKGROUND, MAP_HEIGHT * MAP_WIDTH);
    }
    /* place apple */
    for (int i = 0; i < MAX_APPLE; i++) {
        for (size_t j = 1; j < clients_len; j++) {
            clients[j].map[mapenv->apples[i].y][mapenv->apples[i].x] = APPLE;
        }   
    }
    /* move every players */
    for (size_t i = 1; i < clients_len; i++) {
        /* in fucntion of his direction */
        switch (clients[i].dir) {
            case UP: 
                clients[i].player.y--;
                if (clients[i].player.y < 0)
                    clients[i].player.y = MAP_HEIGHT-1;
            break;
            case DOWN:
                clients[i].player.y++;
                if (clients[i].player.y >= MAP_HEIGHT)
                    clients[i].player.y = 0;
            break;
            case RIGHT: 
                clients[i].player.x++;
                if (clients[i].player.x >= MAP_WIDTH)
                    clients[i].player.x = 0;
            break;
            case LEFT:  
                clients[i].player.x--;
                if (clients[i].player.x < 0)
                    clients[i].player.x = MAP_WIDTH-1;
            break;
            default: break;
        }
        for (int j = 0; j < MAX_APPLE; j++) {
            if (clients[i].player.y == mapenv->apples[j].y && clients[i].player.x == mapenv->apples[j].x) {
                clients[i].snake[clients[i].len].y = clients[i].snake[clients[i].len-1].y;
                clients[i].snake[clients[i].len].x = clients[i].snake[clients[i].len-1].x;
                clients[i].len++;
                if (clients[i].len >= MAX_SNAKE_LENGHT) {
                    printf("%s WON !\n", clients[i].username);
                    send_finish_packets(clients, clients_len, clients[i].username);
                    return -1;
                }
                mapenv->apples[j].y = rand() % MAP_HEIGHT;
                mapenv->apples[j].x = rand() % MAP_WIDTH;
            }
        }
        memmove(&clients[i].snake[1], clients[i].snake, (clients[i].len-1) * sizeof(pos_t));
        clients[i].snake[0].y = clients[i].player.y;
        clients[i].snake[0].x = clients[i].player.x;
        for (int j = 0; j < clients[i].len; j++)
            clients[i].map[clients[i].snake[j].y][clients[i].snake[j].x] = SNAKE;
        for (size_t j = 1; j < clients_len; j++) {
            if (i != j) {
                for (int k = 0; k < clients[i].len; k++)
                    clients[j].map[clients[i].snake[k].y][clients[i].snake[k].x] = ENNEMY;
            }
        }
    }
    return 1;
}

int handle_play(client_t *client) {
    uint8_t *buffer = client->buffer.ptr;
    uint32_t packet_len;
    uint16_t packet_id;
    int dir;
    int i = 0;

    packet_len = *(uint32_t *)buffer;
    i += sizeof(uint32_t);
    packet_id = *(uint16_t *)&buffer[i];
    i += sizeof(uint16_t);
    if (packet_id != PLAY_PACKET_ID)
        return -1;
    dir = *(int *)&buffer[i];
    i += sizeof(int);
    client->buffer.len -= i;
    memmove(buffer, &buffer[i], client->buffer.len);
    if ((uint32_t)i != packet_len) {
        printf("Failed good parse packet\n");
        return -1;
    }
    if (dir != UP && dir != DOWN && dir != LEFT && dir != RIGHT) {
        return -1;
    }
    client->dir = dir;
    return 1;
}