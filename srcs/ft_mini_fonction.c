/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mini_fonction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 16:00:52 by malaine           #+#    #+#             */
/*   Updated: 2016/05/31 14:56:35 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <curses.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <term.h>

void	ft_go_up_bitch(t_line *l)
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

void	ft_go_down_bitch(t_line *l)
{
	l->count++;
	if (((l->count + l->sizeprompt) % l->largeur) == 0)
		do_term("do");
	else
		do_term("nd");
}

void	ft_check_space_r(t_line *l)
{
	while (l->str[l->count] != ' ' && l->str[l->count] != '\0')
		ft_go_down_bitch(l);
}

void	ft_check_is_space_r(t_line *l)
{
	while (l->str[l->count] == ' ')
		ft_go_down_bitch(l);
}

int		ft_check_last_word(t_line *l)
{
	int a;

	a = l->count;
	while (l->str[a] != '\0')
	{
		if (l->str[a] == ' ' || l->str[a] == '\t')
			return (0);
		a++;
	}
	return (-1);
}

int		ft_check_first_word(t_line *l)
{
	int a;

	a = l->count;
	while (a >= 0)
	{
		if (l->str[a] == ' ' || l->str[a] == '\t')
			return (0);
		a--;
	}
	return (-1);
}

void	ft_check_is_space_l(t_line *l)
{
	while (l->str[l->count - 1] == ' ')
		ft_go_up_bitch(l);
}

void	ft_check_space_l(t_line *l)
{
	while (l->str[l->count - 1] != ' ' && l->count != 0)
		ft_go_up_bitch(l);
}
