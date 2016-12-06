#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jguthert <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/30 15:25:02 by jguthert          #+#    #+#              #
#    Updated: 2016/12/06 19:52:44 by agadhgad         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

##=-		Compilatator		-=##

NAME = 21sh

CC_FLAGS = gcc -Wall -Wextra -g3 -Werror

##=-		Rules		-=##

INC_PATH := -I include/ -I libft/includes
LIB_PATH := -L libft/ -lft -ltermcap

##=-		Colors		-=##

RED = \033[1;31m
BLUE = \033[1;34m
GREEN = \033[0;32m
YELLOW = \033[1;33m

##=-		Files Path		-=##

FILES = autocomp/autocomp			\
		autocomp/autocomp_utils		\
		autocomp/autocomp_utils_1	\
		autocomp/autocomp_utils_1_1	\
		autocomp/autocomp_utils_1_2	\
		autocomp/autocomp_utils_2	\
		autocomp/autocomp_utils_3	\
		autocomp/autocomp_utils_4	\
									\
		builtin/alias_bis			\
		builtin/bi_alias			\
		builtin/bi_cd				\
		builtin/bi_clear			\
		builtin/bi_echo				\
		builtin/bi_env				\
		builtin/bi_exit				\
		builtin/bi_export			\
		builtin/bi_getenv 			\
		builtin/bi_history 			\
		builtin/bi_setenv 			\
		builtin/bi_unsetenv			\
		builtin/bi_42info			\
		builtin/bi_glob				\
		builtin/builtin				\
		builtin/get_alias			\
									\
		line_edition/actions		\
		line_edition/check_space	\
		line_edition/check_word		\
		line_edition/cut_cpy_paste	\
		line_edition/do_termcaps	\
		line_edition/ft_ctrl_r		\
		line_edition/ft_init_line	\
		line_edition/ft_insertion	\
		line_edition/ft_key			\
		line_edition/ft_key_2		\
		line_edition/ft_print_key	\
		line_edition/ft_up_down		\
		line_edition/go_up_down		\
		line_edition/go_up_down_2	\
		line_edition/print_ctrl_r	\
		line_edition/read			\
		line_edition/reset_line		\
									\
		parsing/file_get_contents	\
		parsing/parse_history		\
		parsing/parse_line			\
		parsing/parse_quote			\
		parsing/search_history		\
									\
		other/catch_sig				\
		other/check_bin				\
		other/do_fork				\
		other/free_list				\
		other/ft_size_tab			\
		other/garbage_collector		\
		other/get_event				\
		other/heredoc				\
		other/history				\
		other/home_end				\
		other/init_env				\
		other/key_is_special		\
		other/main					\
		other/print_error			\
		other/print_prompt			\
		other/storage_new_var		\
		other/x_strdup				\
		other/x_strjoin				\
		other/x_strnew				\
		other/x_strsplit \
		other/tilde_path \
		other/packets \
								\
		use/f1			\
								\
		glob/glob \
		glob/gbrace \
		glob/gparse \
		glob/gparse2 \
		glob/gparse3 \
		glob/globuse \
		glob/globuse2 \
		glob/globexec \
		glob/globexec2 \
						\
		shell/shell		\
		shell/shell2		\
		shell/shelluse		\
		shell/shelluse2		\
		shell/exec		\
		shell/exec2		\
		shell/exec3		\
		shell/prepare_exec		\
		shell/prepare_exec2		\
		shell/prepare_exec3		\
		\
		parser/parser \
		parser/parser2 \
		parser/parser3 \
		parser/parser4 \
		parser/parser5 \
		parser/parser6 \
		parser/parseruse \
		parser/parseruse2 \
		parser/parseruse3 \
		parser/preparse \
		parser/preparse2 \
		parser/preparse3 \
		parser/redirection \
		parser/redirection2 \
		\
		hash/hash_functions \
		hash/hash \
		hash/init_bin_list \

SRC := $(addprefix srcs/,$(addsuffix .c,$(FILES)))
OBJ := $(addprefix obj/,$(addsuffix .o,$(FILES)))

##=-		Compilation and Linkage		-=##

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(BLUE)Compilation of object files in project directory is complete.\n"
	@echo "$(YELLOW)Recompilation of the library in progress.."
	@make -C libft/
	@echo "$(BLUE)Compilation of the library is complete.\n"
	@echo "$(YELLOW)Linkage of object files with the library is in progress.."
	@$(CC_FLAGS) $(OBJ) $(LIB_PATH) $(INC_PATH) -o $(NAME)
	@echo "$(BLUE)Linkage is complete."
	@echo "$(GREEN)Done."

obj/%.o: srcs/%.c
	@mkdir -p $(dir $@)
	@$(CC_FLAGS) -c $< $(INC_PATH) -o $@

clean:
	@echo "\n$(RED)Deleting object files of the project"
	@$(RM) $(OBJ)
	@make -C libft/ clean

fclean: clean
	@echo "$(RED)Remove the executable\n"
	@$(RM) $(NAME)
	@make -C libft/ fclean

re: fclean all

.PHONY: all clean fclean re
