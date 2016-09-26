/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   go_up_down.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/02 17:22:44 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/19 15:43:58 by malaine          ###   ########.fr       */
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
	if (l->count >= l->largeur)
	{
		do_term("up");
		l->count = l->count - l->largeur;
	}
	else
		ft_home(l);
}

void		ft_boucle_down(int i, int z, t_line *l)
{
	while(z < i)
	{
		do_term("nd");
		l->count++;
		z++;
	}
}

void		ft_first_line_down(t_line *l)
{
	int i;

	i = l->count + l->sizeprompt;
	l->count = l->largeur - l->sizeprompt;
	if(l->count + i <= l->size)
		ft_boucle_down(i, 0, l);
	else
		ft_boucle_down(l->size, l->count, l);
}

void		ft_other_line_down(t_line *l)
{
	int i;
	int z;

	i = (l->count + l->sizeprompt)/l->largeur;
	z = l->count - i*l->largeur + l->sizeprompt;
	l->count = l->count + l->largeur - z;
	ft_boucle_down(z, 0, l);
}

void		ft_ctrl_down(t_line *l)
{
	if (l->count + l->largeur < l->size)
	{
			do_term("do");
		if (l->count < l->largeur - l->sizeprompt)
			ft_first_line_down(l);
		else
			ft_other_line_down(l);
	}
	else
		ft_end(l);
}
