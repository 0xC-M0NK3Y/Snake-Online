#ifndef LOGIN_H
# define LOGIN_H

#include "defines.h"

int handle_logging(uint8_t *buffer, int *buffer_len, char *verif);
int handle_waiting(uint8_t *buffer, int *buffer_len, int *state, map_t *map);
int send_login_packet(socket_t sock, char *username);

#endif /* login.h */