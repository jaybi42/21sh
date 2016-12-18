/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_big_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/18 17:49:13 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/18 19:18:33 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include "shell.h"

static int	print_str(t_line *l, int window)
{
	int		a;
	char	*tmp;

	a = l->start_print;
	tmp = x_strsub(l->str, a, window);
	ft_putstr(tmp);
	xmemdel((void **)&tmp);
	l->count = l->final_count;
	if (l->final_count == l->size)
		return (1);
	return (0);
}

static void	put_cursor(t_line *l, int window)
{
	while (l->count < l->final_count && --window)
	{
		if (l->largeur != 0 && l->count != 0
			&& (l->count + 1) % l->largeur == 0)
			do_term("do");
		else
			do_term("nd");
		l->count++;
	}
}

static void	init_big_print(t_line *l, int window)
{
	if (l->start_print == -1)
	{
		l->start_print = l->size - window;
		l->final_count = l->size;
	}
	else if (l->final_count < l->start_print)
		l->start_print = l->final_count;
	else if (l->final_count > l->start_print + window)
		l->start_print = l->final_count - window;
}

void		big_print(t_line *l, int window)
{
	init_big_print(l, window);
	do_term("cl");
	print_prompt(g_prompt.rand, g_prompt.g_env, g_lenv, g_prompt.l);
	if (l->start_print != 0)
		ft_putstr(". . . .\n");
	do_term("sc");
	if (print_str(l, window) == 1)
		return ;
	do_term("rc");
	put_cursor(l, window);
	if (l->start_print + window < l->size)
		ft_putstr("\n. . . .");
}
