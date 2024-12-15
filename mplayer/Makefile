
SRCS1_CLIENT = zad1_client.c
OBJS1_CLIENT = $(SRCS1_CLIENT:.c=.o)

SRCS1_SERVER = zad1_server.c
OBJS1_SERVER = $(SRCS1_SERVER:.c=.o)

SRCS_MPLAYER = mplayer.c
OBJS_MPLAYER = $(SRCS_MPLAYER:.c=.o)

CFLAGS = -O3 -Wall -Wextra -Werror


.c.o:
	gcc ${CFLAGS} -c $< -o ${<:.c=.o}

all: client server mplayer_frontend

client: ${OBJS1_CLIENT}
	gcc ${OBJS1_CLIENT} -o client

server: ${OBJS1_SERVER}
	gcc ${OBJS1_SERVER} -o server

mplayer_frontend: ${OBJS_MPLAYER}
	gcc ${OBJS_MPLAYER} -o mplayer_frontend

clean:
	rm -f ${OBJS1_SERVER} ${OBJS1_CLIENT} ${OBJS_MPLAYER}

fclean: clean
	rm -f client server mplayer_frontend

re: fclean all

.PHONY: all clean fclean re

