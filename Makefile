NAME_CLIENT = snake-online
NAME_SERVER = snake-online-server
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -g
LDFLAGS = -lSDL2 -lSDL2_ttf
SRC_CLIENT = $(addprefix src_client/, main.c  inits.c  utils.c  login.c  play.c  display.c)
OBJ_CLIENT = ${SRC_CLIENT:.c=.o}
SRC_SERVER = $(addprefix src_server/, main.c  clients.c  utils.c  login.c  start.c  play.c)
OBJ_SERVER = ${SRC_SERVER:.c=.o}

all : ${NAME_CLIENT} ${NAME_SERVER}

${NAME_CLIENT}: ${OBJ_CLIENT}
		${CC} ${OBJ_CLIENT} ${CFLAGS} ${LDFLAGS} -o ${NAME_CLIENT}

${NAME_SERVER}: ${OBJ_SERVER}
		${CC} ${OBJ_SERVER} ${CFLAGS} -o ${NAME_SERVER}

%.o: %.c
		${CC} -c ${<} -o ${@} ${CFLAGS}

clean:
		rm -rf ${OBJ_CLIENT} ${OBJ_SERVER}

fclean: clean
		rm -rf ${NAME_CLIENT} ${NAME_SERVER}

re : fclean all
