#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jguthert <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/30 15:25:02 by jguthert          #+#    #+#              #
#    Updated: 2016/05/25 14:54:51 by jguthert         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

##=-  Compilatator -=##

NAME = 21sh
CC = gcc


##=-  FLAGS -=##

CFLAGS = -Wall -Wextra -g3 #-Werror


##=-  PATH  -=##


##=-  Rules -=##

LIB := libft/libft.a
IFLAGS := -I include/ -I libft/includes
LFLAGS := -L libft/ -lft -lncurses


##=-  Colors  -=##

GREEN:="\033[1;32m"
EOC:="\033[0m"


##=-  Files -=##

FILES =					main			\
						init_env		\
						read			\
						shell			\
						builtin			\
						free_list		\
						bi_env			\
						bi_unsetenv		\
						bi_setenv		\
						bi_cd			\
						bi_exit			\
						bi_getenv		\
						bi_echo			\
						print_error		\
						print_prompt	\
						do_exec			\
						catch_sig		\


SRC := $(addprefix srcs/,$(addsuffix .c,$(FILES)))
OBJ := $(addprefix obj/,$(addsuffix .o,$(FILES)))


##=-  Process -=##

all: $(NAME)

$(NAME): $(OBJ) $(LIB)
	@echo $(GREEN) "Compiling $@" $(EOC)
	@$(CC) $(SRC) $(LFLAGS) $(IFLAGS) $(CFLAGS) -o $(NAME)

$(LIB):
	@make -C libft

obj/%.o: srcs/%.c
	@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $<

clean:
	@echo $(GREEN) "Remove .o  [21sh]" $(EOC)
	@make -C ./libft clean
	@/bin/rm -f $(OBJ)

fclean: clean
	@echo $(GREEN) "Remove all [21sh]" $(EOC)
	@make -C ./libft fclean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re