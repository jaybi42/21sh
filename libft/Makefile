# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ibouchla <ibouchla@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/17 12:21:30 by ibouchla          #+#    #+#              #
#    Updated: 2016/04/14 22:29:55 by ibouchla         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

INC_PATH = -I includes

SRC_PATH = srcs

SRC_NAME = ft_isprint.c ft_memset.c ft_bzero.c			\
			ft_memcpy.c ft_memccpy.c ft_memmove.c 		\
			ft_memchr.c ft_memcmp.c ft_strlen.c 		\
			ft_strdup.c ft_strcpy.c ft_strncpy.c 		\
			ft_strcat.c ft_strncat.c ft_strlcat.c 		\
			ft_strchr.c ft_strrchr.c ft_strstr.c 		\
			ft_strnstr.c ft_strcmp.c ft_strncmp.c 		\
			ft_atoi.c ft_isalpha.c ft_isdigit.c 		\
			ft_isalnum.c ft_isascii.c ft_tablen.c 		\
			ft_toupper.c ft_tolower.c ft_memalloc.c 	\
			ft_memdel.c ft_strnew.c ft_strdel.c 		\
			ft_strclr.c ft_striter.c ft_striteri.c 		\
			ft_strmap.c ft_strmapi.c ft_strequ.c 		\
			ft_strnequ.c ft_strsub.c ft_strjoin.c 		\
			ft_strtrim.c ft_strsplit.c ft_itoa.c 		\
			ft_putchar.c ft_putstr.c ft_putendl.c 		\
			ft_putnbr.c ft_putchar_fd.c ft_putstr_fd.c 	\
			ft_putendl_fd.c ft_putnbr_fd.c ft_lstnew.c 	\
			ft_lstdelone.c ft_lstdel.c ft_lstadd.c 		\
			ft_lstiter.c ft_lstmap.c ft_itoa_base.c 	\
			ft_sqrt.c ft_power.c ft_lstadd_last.c 		\
			ft_strndup.c get_next_line.c ft_getnbr.c	\
			ft_ulltoa_base.c ftv_print.c ftv_insert.c	\
			ftv_del.c ft_error.c ft_nbrlen.c			\
			ft_isblank.c ft_isspace.c ft_tabdel.c		\
			ft_strsplit_space.c ft_strsplit_blank.c		\
			ft_strisalnum.c	ft_print_tab.c ft_listlen.c	\
			ftl_init.c ftl_insert.c ft_memdup.c			\
			ftl_destruct.c ft_strisnum.c 				\
			ft_first_word.c								\
			ft_puttab.c ft_printf.c ft_printf_find.c 	\
			ft_printf_find2.c ft_printf_flag.c 			\
			ft_litoa.c ft_putw.c ft_putc.c 				\
			ft_printf_printv.c ft_printf_printc.c 		\
			ft_printf_utils.c ft_printf_bonus.c 		\
			ft_printf_flag2.c ft_putc2.c ft_latoi.c 	\
			ft_dela_to_b.c ft_tlen.c ft_putstr_dbcote.c \
			a_clear.c  a_event.c  a_handle.c a_handle2.c a_mode.c \
			a_print.c  a_screen.c a_listener.c printf_like.c

CREATE_LIB = ar rc $(NAME) $(OBJET)

ID_LIB = ranlib $(NAME)

CC_FLAGS = gcc -Wall -Wextra -Werror

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))

OBJET = $(SRC:.c=.o)

RED = \033[1;31m

BLUE = \033[1;34m

GREEN = \033[0;32m

YELLOW = \033[1;33m

all: $(NAME)

$(NAME): $(OBJET)
	@echo "$(BLUE)Compilation of object files in libft directory is complete.\n"
	@echo "$(YELLOW)Creation of the library in progress.."
	@$(CREATE_LIB)
	@echo "$(BLUE)Creation of the library is complete.\n"
	@echo "$(YELLOW)Indexing of the library in process.."
	@$(ID_LIB)
	@echo "$(BLUE)Indexing of the library is complete.\n"

%.o: %.c
	@$(CC_FLAGS) -c $< $(INC_PATH) -o $@

clean:
	@echo "\n$(RED)Deleting object files of the library"
	@$(RM) $(OBJET)

fclean: clean
	@echo "$(RED)Remove the library\n"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
