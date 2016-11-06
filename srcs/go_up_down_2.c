/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   go_up_down_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/02 10:52:44 by malaine           #+#    #+#             */
/*   Updated: 2016/11/02 10:54:55 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

void		go_up(t_line *l)
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

void		go_down(t_line *l)
{
	l->count++;
	if (((l->count + l->sizeprompt) % l->largeur) == 0)
		do_term("do");
	else
		do_term("nd");
}
