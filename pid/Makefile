SRCS = main.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -O3 -Wall -Wextra -Werror
NAME = program

.c.o:
	$(CC) ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	$(CC) ${OBJS} -o ${NAME}
all: ${NAME}

clean:
	rm -f ${OBJS}
fclean: clean
	rm -f ${NAME}
re: fclean all

.PHONY: all clean fclean re
