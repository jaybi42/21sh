/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 13:16:09 by malaine           #+#    #+#             */
/*   Updated: 2016/06/08 14:39:25 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

void		ft_left(t_line *l)
{
	if (l->count != 0)
		go_up(l);
}

void		ft_right(t_line *l)
{
	if (l->count < l->size)
		go_down(l);
}

void		ft_backspace(t_line *l)
{
	int		tmp;
	int		nblines;

	nblines = (l->size + l->sizeprompt) / l->largeur;
	if (l->count > 0)
	{
		go_up(l);
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
	if (check_last_word(l) == 0)
	{
		if (l->str[l->count] == ' ')
			check_is_space_r(l);
		else
		{
			check_space_r(l);
			check_is_space_r(l);
		}
	}
	else
		check_space_r(l);
}

void		ft_ctrl_l(t_line *l)
{
	if (l->str[l->count - 1] != ' ')
		check_space_l(l);
	else
	{
		check_is_space_l(l);
		check_space_l(l);
	}
}
