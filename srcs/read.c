/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 16:07:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/06 19:03:07 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>

void	debug_editline(t_line *l)
{
	do_term("sc");
	do_goto("DO", 0, 5);
	do_term("cd");
	do_goto("DO", 0, 5);
	ft_putstr("count = ");
	ft_putnbr(l->count);
	ft_putstr(", largeur = ");
	ft_putnbr(l->largeur);
	ft_putstr(", size = ");
	ft_putnbr(l->size);
	ft_putstr(", sizeprompt = ");
	ft_putnbr(l->sizeprompt);
	ft_putstr("\nSTR = ");
	ft_putstr(l->str);
	do_term("rc");
}

t_av	**read_init(t_line *l, t_ftl_root *hist)
{
	g_line = l;
	ft_init_line(l, hist);
	while (1)
	{
		ft_bzero(l->buffer, 6);
		if (read(0, l->buffer, 6) == -1)
			return (NULL);
		if (l->buffer[0] != 10)
			actions(l);
		if (l->buffer[0] != 10 && ft_isprint(l->buffer[0]) == 1)
			ft_print_key(l);
		if (l->buffer[0] == 10)
		{
			ft_putchar('\n');
			break ;
		}
	}
	if (parse_line(l, hist) == 0)
		return (parse_commands(l->str));
	return (parse_commands(""));
}
