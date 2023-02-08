#ifndef START_H
# define START_H

#include <stddef.h>

#include "structs.h"

int send_start_packets(client_t *clients, size_t client_len, map_env_t *mapenv);

#endif /* start.h */