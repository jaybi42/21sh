/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   go_up_down.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/02 17:22:44 by jguthert          #+#    #+#             */
/*   Updated: 2016/06/06 19:09:59 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

void	go_up(t_line *l)
{
	if (((l->count + l->sizeprompt) % l->largeur) == 0)
	{
		do_term("up");
		do_goto("ch", 0, l->largeur);
	}
	else
		do_term("le");
	l->count--;
}

void	go_down(t_line *l)
{
	l->count++;
	if (((l->count + l->sizeprompt) % l->largeur) == 0)
		do_term("do");
	else
		do_term("nd");
}

void		ft_ctrl_up(t_line *l)
{
	(void)l;
	do_term("up");
}

void		ft_ctrl_down(t_line *l)
{
	if (l->largeur < l->size && (l->count + l->largeur) < l->size)
	{
		do_term("hd");
		l->count = l->size - l->count;
	}
}
