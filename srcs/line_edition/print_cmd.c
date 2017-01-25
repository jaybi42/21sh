/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/18 16:59:38 by jguthert          #+#    #+#             */
/*   Updated: 2017/01/18 17:11:40 by malaine          ###   ########.fr       */
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

static void	toomuch_print(t_line *l)
{
	do_term("cl");
	print_prompt(g_prompt.rand, g_prompt.g_env, g_lenv, g_prompt.l);
	l->sizeprompt = l->sizeprompt + 3;
	l->final_count = ft_strlen(l->str);
	l->count = 0;
	print_str(l);
}

static void	little_print(t_line *l)
{
	if (l->resize == 1)
	{
		l->count = 0;
		l->resize = 0;
	}
	else
		ft_home(l);
	do_term("cd");
	print_str(l);
	ft_home(l);
	put_cursor(l);
}

void		print(t_line *l)
{
	int window;

	window = (l->hauteur - 1) * l->largeur;
	if (l->size >= window - l->sizeprompt)
		toomuch_print(l);
	else
		little_print(l);
}
