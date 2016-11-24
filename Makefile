#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jguthert <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/30 15:25:02 by jguthert          #+#    #+#              #
#    Updated: 2016/11/24 18:24:45 by ibouchla         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

##=-		Compilatator		-=##

NAME = 21sh

CC_FLAGS = gcc -Wall -Wextra -g3 #-Werror

##=-		Rules		-=##

INC_PATH := -I include/ -I libft/includes
LIB_PATH := -L libft/ -lft -ltermcap

##=-		Colors		-=##

RED = \033[1;31m
BLUE = \033[1;34m
GREEN = \033[0;32m
YELLOW = \033[1;33m

##=-		Root files		-=##

FILES =					main				\
						init_env			\
						read				\
						shell				\
						builtin				\
						free_list			\
						bi_export			\
						bi_env				\
						bi_unsetenv			\
						bi_setenv			\
						bi_cd				\
						bi_exit				\
						bi_getenv			\
						bi_echo				\
						bi_history			\
						print_error			\
						print_prompt		\
						catch_sig			\
						history				\
						do_fork				\
						get_alias			\
						check_bin			\
						storage_new_var		\
						heredoc				\
						actions				\
						do_termcaps			\
						ft_up_down			\
						go_up_down_2		\
						go_up_down			\
						ft_key				\
						ft_key_2			\
						ft_init_line		\
						ft_insertion		\
						ft_print_key		\
						home_end			\
						cut_cpy_paste		\
						check_space			\
						check_word			\
						autocomp			\
						autocomp_utils		\
						autocomp_utils_1	\
						autocomp_utils_2	\
						autocomp_utils_3	\
						autocomp_utils_4	\
						parse_quote			\
						parse_history		\
						get_event			\
						search_history		\
						parse_line			\
						storage_all_cmds	\
						garbage_collector	\
						glob 				\
						ft_ctrl_r			\
						print_ctrl_r		\
						key_is_special		\
						bi_clear			\
						reset_line			\
						ft_size_tab			\
						x_strsplit 			\
						x_strjoin 			\
						x_strdup 			\
						x_strnew 			\
						file_get_contents	\
						bi_alias			\
						alias_bis

SRC := $(addprefix srcs/,$(addsuffix .c,$(FILES)))
OBJ := $(addprefix obj/,$(addsuffix .o,$(FILES)))

##=-		Compilation and Linkage		-=##

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(BLUE)Compilation of object files in project directory is complete.\n"
	@echo "$(YELLOW)Recompilation of the library in progress.."
	@make -C libft/ re
	@echo "$(BLUE)Compilation of the library is complete.\n"
	@echo "$(YELLOW)Linkage of object files with the library is in progress.."
	@$(CC_FLAGS) $(OBJ) $(LIB_PATH) $(INC_PATH) -o $(NAME)
	@echo "$(BLUE)Linkage is complete."
	@make -C libft/ fclean
	@echo "$(GREEN)Done."

obj/%.o: srcs/%.c
	@mkdir -p $(dir $@)
	@$(CC_FLAGS) -c $< $(INC_PATH) -o $@

clean:
	@echo "\n$(RED)Deleting object files of the project"
	@$(RM) $(OBJ)

fclean: clean
	@echo "$(RED)Remove the executable\n"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
