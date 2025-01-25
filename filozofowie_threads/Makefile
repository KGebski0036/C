SRCS = filo.c filo_helper.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -O3 -Wall -Wextra -Werror -pthread
LFLAGS = -pthread
NAME = filo

.c.o:
	$(CC) ${CFLAGS} -c $< -o ${<:.c=.o} -I.

${NAME}: ${OBJS}
	$(CC) ${OBJS} -o ${NAME} ${LFLAGS}
all: ${NAME}

clean:
	rm -f ${OBJS}
fclean: clean
	rm -f ${NAME}
re: fclean all

.PHONY: all clean fclean re
