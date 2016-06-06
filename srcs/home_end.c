/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_home_end.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 17:35:38 by malaine           #+#    #+#             */
/*   Updated: 2016/06/06 19:12:28 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

void	ft_home(t_line *l)
{
	l->nbline = (l->count + l->sizeprompt) / l->largeur;
	do_goto("ch", 0, l->sizeprompt);
	if (l->nbline != 0)
		do_goto("UP", l->nbline, l->nbline);
	l->count = 0;
}

void	ft_end(t_line *l)
{
	while (l->count < l->size)
		go_down(l);
}
