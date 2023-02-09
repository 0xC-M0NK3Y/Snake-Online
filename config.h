#ifndef CONFIG_H
# define CONFIG_H

#define MAX_PACKET_LEN (1<<14) // MAXIMUM PACKET LENGTH, MUST BE MINIMUM SIZE OF PLAY PACKET (S -> C) (see Protocole)

#define PLAYER_NEEDED 2 // NUMBER OF PLAYER NEEDED FOR THE GAME TO START

#define WIN_WIDTH 1000 // WIDTH OF THE WINDOW
#define WIN_HEIGHT 800 // HEIGHT OF THE WINDOW

#define SQUARE_SIZE 15 // SIZE OF THE SQUARES
#define BORDER_SIZE 2 // SIZE OF THE BORDURE

#define MAX_SNAKE_LENGHT    5 // MAXIMUM LENGTH OF SNAKE THEN GAME IS ENDED 
#define MAX_APPLE           100 // MAXIMUM OF SIMULTANEOUS APPLE

/* Its better to not touch the other configuration defines */

#define MAP_WIDTH   (WIN_WIDTH/SQUARE_SIZE)
#define MAP_HEIGHT  (WIN_HEIGHT/SQUARE_SIZE)

#define LOGGING     1
#define WAITING     2
#define PLAYING     3
#define FINISHED    4

#define LOGIN_PACKET_ID     0x1
#define CONFIRM_PACKET_ID   0x2
#define START_PACKET_ID     0x3
#define PLAY_PACKET_ID      0x4
#define FINISH_PACKET_ID    0x5

#define BACKGROUND  '0'
#define SNAKE       '1'
#define APPLE       '2'
#define ENNEMY      '3'

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

#endif /* config.h */
