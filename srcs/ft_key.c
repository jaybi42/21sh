/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 13:16:09 by malaine           #+#    #+#             */
/*   Updated: 2016/09/16 16:23:46 by jguthert         ###   ########.fr       */
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

void		ft_ctrl_l(t_line *l)
{
	(void)l;
}
