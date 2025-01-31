CLIENT_TCP_SRCS = tcp_client.c
CLIENT_TCP_OBJS = $(CLIENT_TCP_SRCS:.c=.o)

CLIENT_UDP_SRCS = udp_client.c
CLIENT_UDP_OBJS = $(CLIENT_UDP_SRCS:.c=.o)

SERVER_SRCS = serwer.c
SERVER_OBJS = $(SERVER_SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -pthread
LFLAGS = -pthread

.c.o:
	$(CC) ${CFLAGS} -c $< -o ${<:.c=.o} -I.

all: client_tcp client_udp server

client_tcp: ${CLIENT_TCP_OBJS}
	$(CC) ${LFLAGS} ${CLIENT_TCP_OBJS} -o client_tcp

client_udp: ${CLIENT_UDP_OBJS}
	$(CC) ${LFLAGS} ${CLIENT_UDP_OBJS} -o client_udp
	
server: ${SERVER_OBJS}
	$(CC) ${LFLAGS} ${SERVER_OBJS} -o server

clean:
	rm -f ${CLIENT_TCP_OBJS} ${CLIENT_UDP_OBJS} ${SERVER_OBJS}

fclean: clean
	rm -f client_tcp client_udp server

re: fclean all

.PHONY: all clean fclean re
