#ifndef DEFINES_H
# define DEFINES_H

#include <stdint.h>

typedef int                 socket_t;
typedef uint32_t            ip_t;
typedef uint16_t            port_t;
typedef struct pollfd       pollfd_t;
typedef struct sockaddr_in  sockaddr_in_t;
typedef uint32_t            flag_t;

#define TRUE    1
#define FALSE   0

#endif /* defines.h */