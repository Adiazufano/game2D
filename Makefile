NAME=so_long
CC=cc
CFLAGS=-Wall -Wextra -Werror -Wunreachable-code -Ofast -g
LIBMLX=MLX42

HEADERS=-I ./include -I $(LIBMLX)/inlcude
LIBS =$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm
SRCS=main.c get_next_line.c get_next_line_utils.c
OBJS=$(SRCS:%.c=%.o)

all: libmlx $(NAME)

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(NAME):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -Imlx -Iincludes -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean:clean
	rm -rf $(NAME)

re:fclean all

.PHONY: all clean fclean re

