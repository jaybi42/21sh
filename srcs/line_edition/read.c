/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 16:07:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/29 14:36:29 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>
#include "edit_line.h"

extern t_prompt g_prompt;

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

void	ft_ctrl_l(t_line *l)
{
	do_term("cl");
	if (l->size == 0)
		l->final_count = 0;
	print_prompt(g_prompt.rand, g_prompt.g_env, g_prompt.l_env, g_prompt.l);
	l->sizeprompt = l->sizeprompt + 3;
	print(l);
}

void	ft_ctrl_d(t_line *l)
{
	if (l->size != 0)
		ft_real_backspace(l);
	else
	{
		ft_putchar('\n');
		reset_line();
		exit(0);
	}
}

t_av	**read_init(t_line *l, t_ftl_root *hist)
{
	t_av	**av;
	int		ret;

	g_line = l;
	ft_init_line(l, hist);
	while (42)
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
	ret = parse_line(l, hist);
	av = parse_commands(ret == 0 ? l->str : "");
//	ft_strdel(&l->str);
	return (av);
}
