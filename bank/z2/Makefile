SRCS = bank.c bank_helper.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra -Werror -pthread
LFLAGS = -pthread
NAME = bank

.c.o:
	gcc ${CFLAGS} -c $< -o ${<:.c=.o} -I.

${NAME}: ${OBJS}
	gcc ${LFLAGS} ${OBJS} -o ${NAME}
all: ${NAME}

clean:
	rm -f ${OBJS}
fclean: clean
	rm -f ${NAME}
re: fclean all

.PHONY: all clean fclean re
