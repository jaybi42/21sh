/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_home_end.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 17:35:38 by malaine           #+#    #+#             */
/*   Updated: 2017/01/18 18:24:09 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

static int		count_n_char(char *str, size_t n, char c)
{
	int		i;

	i = 0;
	while (*str != '\0' && n--)
	{
		if (*str == c)
			i++;
		str++;
	}
	return (i);
}

void			ft_home(t_line *l)
{
	l->nbline = (l->count + l->sizeprompt) / (l->largeur ? l->largeur : 1);
	l->nbline += count_n_char(l->str, (size_t)l->count, '\n');
	do_goto("ch", 0, l->sizeprompt);
	if (l->nbline != 0)
		do_goto("UP", l->nbline, l->nbline);
	l->count = 0;
}

void			ft_end(t_line *l)
{
	while (l->count < l->size)
		go_down(l);
}
