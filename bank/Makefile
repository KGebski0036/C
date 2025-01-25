SRCS = bank.c bank_helper.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra -Werror -pthread -g
LFLAGS = -pthread -g
NAME = bank

.c.o:
	$(CC) ${CFLAGS} -c $< -o ${<:.c=.o} -I.

${NAME}: ${OBJS}
	$(CC) ${LFLAGS} ${OBJS} -o ${NAME}
all: ${NAME}

clean:
	rm -f ${OBJS}
fclean: clean
	rm -f ${NAME}
re: fclean all

.PHONY: all clean fclean re
