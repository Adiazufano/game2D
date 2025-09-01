NAME=so_long
CC=cc
CFLAGS=-Wall -Wextra -Werror -Wunreachable-code -Ofast -g
LIBMLX=MLX42

HEADERS=-I ./include -I $(LIBMLX)/inlcude
LIBS =$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm
SRCS=main.c get_next_line.c collectionables.c draw_items.c exit.c flood_fil.c image.c map_errors.c moves.c utils.c
OBJS=$(SRCS:%.c=%.o)
PRINTF_DIR = printf
PRINTF = $(PRINTF_DIR)/libftprintf.a

all: libmlx $(NAME)

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(NAME):$(OBJS)
	@make -C $(PRINTF_DIR)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(PRINTF) $(HEADERS) -o $(NAME)

%.o:%.c
	@$(CC) $(CFLAGS) -Imlx -Iincludes -c $< -o $@

clean:
	@make clean -C $(PRINTF_DIR)
	@rm -rf $(OBJS)

fclean:clean
	@make fclean -C $(PRINTF_DIR)
	@rm -rf $(NAME)

re:fclean all

.PHONY: all libmlx clean fclean re

