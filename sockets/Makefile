CLIENT_SRCS = client.c
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

SERVER_SRCS = serwer.c
SERVER_OBJS = $(SERVER_SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror

.c.o:
	$(CC) ${CFLAGS} -c $< -o ${<:.c=.o} -I.

all: client server

client: ${CLIENT_OBJS}
	$(CC) ${LFLAGS} ${CLIENT_OBJS} -o client
	
server: ${SERVER_OBJS}
	$(CC) ${LFLAGS} ${SERVER_OBJS} -o server

clean:
	rm -f ${CLIENT_OBJS} ${SERVER_OBJS}

fclean: clean
	rm -f client server

re: fclean all

.PHONY: all clean fclean re
