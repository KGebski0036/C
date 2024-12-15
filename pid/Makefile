
SRCS = main.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -O3 -Wall -Wextra -Werror
NAME = program

.c.o:
	gcc ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	gcc ${OBJS} -o ${NAME}
all: ${NAME}

clean:
	rm -f ${OBJS}
fclean: clean
	rm -f ${NAME}
re: fclean all

.PHONY: all clean fclean re

