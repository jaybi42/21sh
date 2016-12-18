/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/18 16:59:38 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/18 19:16:54 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include "shell.h"

static void	print_str(t_line *l)
{
	int a;

	a = l->start_print;
	while (l->str[++a] != '\0')
	{
		ft_putchar(l->str[a]);
		if (l->largeur != 0 && l->count != 0
			&& (l->count + l->sizeprompt + 1) % l->largeur == 0)
			do_term("do");
		l->count++;
	}
}

static void	put_cursor(t_line *l)
{
	while (l->count < l->final_count)
	{
		if (l->largeur != 0 && l->count != 0
			&& (l->count + l->sizeprompt + 1) % l->largeur == 0)
			do_term("do");
		else
			do_term("nd");
		l->count++;
	}
}

static void	little_print(t_line *l)
{
	ft_home(l);
	do_term("cd");
	print_str(l);
	ft_home(l);
	put_cursor(l);
}

void		print(t_line *l)
{
	int window;

	window = (l->hauteur - 4) * l->largeur;
	if (l->size > window)
	{
		ft_putendl("TOO BIG");
		if (false)
			big_print(l, window);
	}
	else
		little_print(l);
}
