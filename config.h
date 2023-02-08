#ifndef CONFIG_H
# define CONFIG_H

#define MAX_PACKET_LEN (1<<14)

#define PLAYER_NEEDED 3

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define SQUARE_SIZE 15
#define BORDER_SIZE 2

#define MAP_WIDTH   (WIN_WIDTH/SQUARE_SIZE)
#define MAP_HEIGHT  (WIN_HEIGHT/SQUARE_SIZE)

#define LOGGING 1
#define WAITING 2
#define PLAYING 3

#define LOGIN_PACKET_ID     0x1
#define CONFIRM_PACKET_ID   0x2
#define START_PACKET_ID     0x3
#define PLAY_PACKET_ID      0x4

#define BACKGROUND  '0'
#define SNAKE       '1'
#define APPLE       '2'
#define ENNEMY      '3'

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

#define MAX_SNAKE_LENGHT    40
#define MAX_APPLE           3

#endif /* config.h */