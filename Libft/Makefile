# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/30 16:13:12 by shunwata          #+#    #+#              #
#    Updated: 2025/07/18 21:36:25 by shunwata         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC_DIR = srcs
INC_DIR = includes

FT_PRINTF_DIR = $(SRC_DIR)/ft_printf
GET_NEXT_LINE_DIR = $(SRC_DIR)/get_next_line

SRCS = $(addprefix $(SRC_DIR)/, ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	   ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
	   ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
	   ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c \
	   ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c \
	   ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c)
OBJS = $(SRCS:.c=.o)

FT_PRINTF_SRCS = $(addprefix $(FT_PRINTF_DIR)/, ft_printf.c ft_printf_utils.c)
FT_PRINTF_OBJS = $(FT_PRINTF_SRCS:.c=.o)

GET_NEXT_LINE_SRCS = $(addprefix $(GET_NEXT_LINE_DIR)/, get_next_line.c)
GET_NEXT_LINE_OBJS = $(GET_NEXT_LINE_SRCS:.c=.o)

BONUS_SRCS = $(addprefix $(SRC_DIR)/, ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c \
			 ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(INC_DIR) -I $(FT_PRINTF_DIR) -I $(GET_NEXT_LINE_DIR)

all: $(NAME)

$(NAME): $(OBJS) $(FT_PRINTF_OBJS) $(GET_NEXT_LINE_OBJS)
	ar rcs $(NAME) $(OBJS) $(FT_PRINTF_OBJS) $(GET_NEXT_LINE_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(FT_PRINTF_OBJS) $(GET_NEXT_LINE_OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: $(NAME) $(BONUS_OBJS)
	ar rcs $(NAME) $(OBJS) $(FT_PRINTF_OBJS) $(GET_NEXT_LINE_OBJS) $(BONUS_OBJS)

.PHONY: all clean fclean re bonus
