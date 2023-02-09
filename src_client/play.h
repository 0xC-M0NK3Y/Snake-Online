#ifndef PLAY_H
# define PLAY_H

#include "structs.h"

int handle_play(uint8_t *buffer, int *buffer_len, map_t *map, param_t *param, conn_manager_t *conn);
int change_direction(const map_t *map, int dir, socket_t sock);

#endif /* play.h */