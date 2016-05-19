/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/08 17:56:10 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/19 11:55:33 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_21SH_H
# define FT_21SH_H

/*
** TODO: Suppr stdio
*/

#include <stdio.h> //

# include "libft.h"
# include <termios.h>
# include <term.h>
# include <curses.h>
# include <sys/ioctl.h>

# define TERM		struct termios

# define H_RED		"\033[1;91m"
# define H_BLUE		"\033[1;94m"
# define END		"\033[0m"

typedef struct		s_sig
{
	t_ftl_root		*root;
}					t_sig;

typedef struct		s_elem
{
	t_ftl_node		node;
	char			*name;
	bool			cursor;
}					t_elem;

typedef void		(*t_modl) (t_ftl_root *root, t_ftl_node **pos);

typedef struct		s_actions
{
	t_modl			action;
	int				value[4];
	char			*key;
}					t_actions;

typedef struct		s_error
{
	bool			cmd;
	char			*str;
	bool			arg;
}					t_error;

/*
**Name: Launch 21sh
**File: ft_21sh.c
**Desc: Parse 21sh
*/

int					ft_21sh(char **av, int ac);

/*
**Name: actions
**File: arrow_actions.c mod_actions.c
**Desc: actions fonctions
*/

int					actions(char *buf, t_ftl_root *root, t_ftl_node **pos);
void				up_action(t_ftl_root *root, t_ftl_node **pos);
void				down_action(t_ftl_root *root, t_ftl_node **pos);
void				right_action(t_ftl_root *root, t_ftl_node **pos);
void				left_action(t_ftl_root *root, t_ftl_node **pos);
void				del_action(t_ftl_root *root, t_ftl_node **pos);
void				sel_action(t_ftl_root *root, t_ftl_node **pos);

/*
**Name: Terminal configuration
**File: c_term.c
**Desc: configure the terminal
*/

int					init_term(void);
int					reset_term(void);

/*
**Name: Print
**File: print_term.c int_putchar.c padding_control.c do_termcap.c
**Desc: Print ft_select
*/

int					int_putchar(int c);
int					do_termcap(char *key);
int					move_termcap(char *key, int col, int row);

/*
**Name: List
**File: mod_list.c
**Desc: Parse list
*/



/*
**Name: Error
**File: print_error.c
**Desc: Parce error
*/

int					print_error(char *cmd, int error);

/*
**Name: Signals
**File: catch_sig.c
**Desc: catch the signals
*/

void				catch_sig(t_ftl_root *root);
void				is_sig(int signum);

#endif
