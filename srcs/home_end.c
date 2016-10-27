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

static int  count_n_char(char *str, size_t n, char c)
{
    int     i;

    i = 0;
    while (*str != '\0' && n--)
    {
        if (*str == c)
            i++;
        str++;
    }
    return (i);
}

int			get_nbline(t_line *l)
{
	int		i;

	if (l->largeur == 0)
		return (0);
	i = (l->count + l->sizeprompt) / l->largeur;
	i += count_n_char(l->str, (size_t)l->count, '\n');
	return (i);
}

void	ft_home(t_line *l)
{
	do_goto("ch", 0, l->sizeprompt);
	if (l->nbline != 0)
		do_goto("UP", l->nbline, l->nbline);
	l->nbline = get_nbline(l);
	l->count = 0;
}

void	ft_end(t_line *l)
{
	while (l->count < l->size)
		go_down(l);
}
