# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/18 21:38:29 by shunwata          #+#    #+#              #
#    Updated: 2025/07/19 14:00:34 by shunwata         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(LIBFT_DIR)/includes -I $(FT_PRINTF_DIR) -I $(GET_NEXT_LINE_DIR)

SRC = pipex.c
OBJ = $(SRC:.c=.o)

LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_DIR = Libft

FT_PRINTF_DIR = $(LIBFT_DIR)/srcs/ft_printf
GET_NEXT_LINE_DIR = $(LIBFT_DIR)/srcs/get_next_line

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
