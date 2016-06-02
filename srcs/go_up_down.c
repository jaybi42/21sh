/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   go_up_down.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/02 17:22:44 by jguthert          #+#    #+#             */
/*   Updated: 2016/06/02 17:23:30 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

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
