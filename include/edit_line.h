/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_line.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/08 17:56:10 by jguthert          #+#    #+#             */
/*   Updated: 2016/08/30 17:35:57 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDIT_LINE_H
# define EDIT_LINE_H

/*
** TODO: Suppr stdio
*/

# include "libft.h"
# include <sys/ioctl.h>

# define H_RED		"\033[1;91m"
# define H_BLUE		"\033[1;94m"
# define END		"\033[0m"

# define TIOCGETD	_IOR('t', 26, int)

typedef struct	s_line
{
	int			nbline;
	int			line1;
	int			sizeprompt;
	int			hauteur;
	int			largeur;
	int			var;
	char		buffer[6];
	char		*str;
	char		*strcpy;
	int			count;
	int			tmp;
	int			size;
	t_ftl_root	*hist;
	int			hist_size;
}				t_line;

typedef struct	s_elem
{
	t_ftl_node	node;
	char		*name;
	int			name_len;
	int			pos;
	bool		selected;
	bool		cursor;
}				t_elem;

typedef void	(*t_spe_key) (t_line *l);

typedef struct	s_actions
{
	t_spe_key	action;
	int			value[6];
	char		*key;
}				t_actions;

/*
**Name: Init
**File:
**Desc: initialize stuff to start editing line
*/

int				init_env(t_list **g_env, t_list **l_env);
void			ft_init_line(t_line *l, t_ftl_root *hist);
void			ft_init_read(t_line *l);
int				init_term(void);

/*
**Name: Actions
**File:
**Desc: Actions you do once you enter a special Key with the keybord
*/

int				actions(t_line *l);
void			ft_backspace(t_line *l);
void			ft_right(t_line *l);
void			ft_left(t_line *l);
void			ft_ctrl_down(t_line *l);
void			ft_ctrl_up(t_line *l);
void			ft_paste(t_line *l);
void			ft_cut(t_line *l);
void			ft_home(t_line *l);
void			ft_end(t_line *l);
void			ft_ctrl_r(t_line *l);
void			ft_ctrl_l(t_line *l);

/*
**Name: Termcaps
**File:
**Desc: Termcaps actions
*/

int				do_goto(char *key, int col, int row);
int				do_term(char *key);
void			go_up(t_line *l);
void			go_down(t_line *l);
void			ft_paste_print(t_line *l, int var, int tmp);
char			*ft_delete_char(int cursor, char *str, int size);
char			*ft_insertion(int cursor, char *str, int size, char c);

/*
**Name: Check
**File:
**Desc: Check functions
*/

int				check_first_word(t_line *l);
int				check_last_word(t_line *l);
void			check_is_space_l(t_line *l);
void			check_space_l(t_line *l);
void			check_space_r(t_line *l);
void			check_is_space_r(t_line *l);

/*
**Name: Print
**File:
**Desc: Print functions
*/

int				int_putchar(int c);
void			ft_print_first(t_line *l);
void			ft_print_rest(t_line *l);
void			ft_print_insert(t_line *l);
void			ft_print_key(t_line *l);

#endif
