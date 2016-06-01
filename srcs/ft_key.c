/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 13:16:09 by malaine           #+#    #+#             */
/*   Updated: 2016/05/31 14:57:36 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <curses.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <term.h>
#include <sys/ioctl.h>
#include <stdlib.h>

void		ft_ctrl_up(t_line *l)
{
	tputs(tgetstr("up", NULL), 1, putchar_s);
}

void		ft_ctrl_down(t_line *l)
{
	if (largeur < l->size && (l->count + largeur) < l->size)
	{
		tputs(tgetstr("hd", NULL), 1, putchar_s);
		l->count = l->size - l->count;
	}
}

void		ft_left(t_line *l)
{
	if (l->count != 0)
		ft_go_up_bitch(l);
}

void		ft_right(t_line *l)
{
	if (l->count < l->size)
		ft_go_down_bitch(l);
}

void		ft_backspace(t_line *l)
{
	int		tmp;
	int		nblines;

	nblines = (l->size + l->sizeprompt) / l->largeur;
	if (l->count > 0)
	{
		ft_go_up_bitch(l);
		l->size--;
		l->str = ft_delete_char(l->count, l->str, l->size);
		if (nblines == 0)
			do_term("dc");
		else
		{
			do_term("cd");
			do_term("sc");
			tmp = l->count;
			while (l->str[tmp] != '\0')
			{
				ft_putchar(l->str[tmp]);
				tmp++;
			}
			do_term("rc");
		}
	}
}

void		ft_ctrl_r(t_line *l)
{
	if (ft_check_last_word(l) == 0)
	{
		if (l->str[l->count] == ' ')
			ft_check_is_space_r(l);
		else
		{
			ft_check_space_r(l);
			ft_check_is_space_r(l);
		}
	}
	else
		ft_check_space_r(l);
}

void		ft_ctrl_l(t_line *l)
{
	if (l->str[l->count - 1] != ' ')
		ft_check_space_l(l);
	else
	{
		ft_check_is_space_l(l);
		ft_check_space_l(l);
	}
}
