#ifndef PLAY_H
# define PLAY_H

#include <stddef.h>

#include "structs.h"

int send_play_packets(client_t *clients, size_t clients_len);
int move_players(client_t *clients, size_t clients_len, map_env_t *mapenv);
int handle_play(client_t *client);

#endif /* play.h */