/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/21 11:39:33 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/08 16:07:18 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <signal.h>

static void		get_color(int rand)
{
	int const	color[] = {31, 32, 33, 34, 35, 36, 37};
	int const	style[] = {0, 1};

	ft_putstr("\033[");
	ft_putnbr(style[rand % 2]);
	ft_putstr(";");
	ft_putnbr(color[rand % 6]);
	ft_putstr("m");
}

static	int		print_value(t_list *g_env, t_list *g_lenv, char *key)
{
	t_list		*temp;

	temp = g_env;
	while (temp != NULL)
	{
		if (ft_strcmp(((t_env *)temp->content)->name, key) == 0)
		{
			ft_putstr(((t_env *)temp->content)->value);
			return (ft_strlen(((t_env *)temp->content)->value));
		}
		temp = temp->next;
	}
	temp = g_lenv;
	while (temp != NULL)
	{
		if (ft_strcmp(((t_env *)temp->content)->name, key) == 0)
		{
			ft_putstr(((t_env *)temp->content)->value);
			return (ft_strlen(((t_env *)temp->content)->value));
		}
		temp = temp->next;
	}
	return (0);
}

void			print_prompt(int rand, t_list *g_env, t_list *g_lenv, t_line *l)
{
	get_color(rand);
	if (print_value(g_env, g_lenv, "PWD") == 0)
		ft_putstr("21sh");
	ft_putstr("\033[0m");
	get_color(rand + 2);
	ft_putchar('\n');
	ft_putchar('@');
	l->sizeprompt = print_value(g_env, g_lenv, "USER");
	ft_putstr("$>");
	ft_putstr("\033[0m");
}
