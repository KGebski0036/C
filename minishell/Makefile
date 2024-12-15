
SRCS1a = pipe_1a.c
OBJS1a = $(SRCS1a:.c=.o)

SRCS1b = pipe_1b.c
OBJS1b = $(SRCS1b:.c=.o)

SRCS_MINISHELL = minishell.c
OBJS_MINISHELL = $(SRCS_MINISHELL:.c=.o)

CFLAGS = -O3 -Wall -Wextra -Werror


.c.o:
	gcc ${CFLAGS} -c $< -o ${<:.c=.o}

1a: ${OBJS1a}
	gcc ${OBJS1a} -o 1a

1b: ${OBJS1b}
	gcc ${OBJS1b} -o 1b

minishell: ${OBJS_MINISHELL}
	gcc ${OBJS_MINISHELL} -o minishell

all: 1a 1b minishell

clean:
	rm -f ${OBJS1a} ${OBJS1b} ${OBJS_MINISHELL}
fclean: clean
	rm -f 1a 1b minishell
re: fclean all

.PHONY: all clean fclean re

