
SRCS1_READER = reader.c
OBJS1_READER = $(SRCS1_READER:.c=.o)

SRCS1_WRITER = writer.c
OBJS1_WRITER = $(SRCS1_WRITER:.c=.o)

CFLAGS = -O3 -Wall -Wextra -Werror


.c.o:
	gcc ${CFLAGS} -c $< -o ${<:.c=.o} -I .

all: reader writer

reader: ${OBJS1_READER}
	gcc ${OBJS1_READER} -o reader

writer: ${OBJS1_WRITER}
	gcc ${OBJS1_WRITER} -o writer

clean:
	rm -f ${OBJS1_READER} ${OBJS1_WRITER}

fclean: clean
	rm -f reader writer

re: fclean all

.PHONY: all clean fclean re

