/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseruse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:56:30 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/08 20:49:10 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	delete_c(t_av **cmd, int id_argv, int i, int *pi)
{
	int a;

	a = 0;
	while ((*cmd)->argv[id_argv][i + a])
	{
		(*cmd)->argv[id_argv][i + a] = (*cmd)->argv[id_argv][i + a + 1];
		(*cmd)->argv_auth[id_argv][i + a] =
		(*cmd)->argv_auth[id_argv][i + a + 1];
		a++;
	}
	(*pi) -= 1;
}

void	delete_s(t_av **cmd, int id_argv)
{
	int a;

	a = id_argv;
	while ((*cmd)->argv[a])
	{
		(*cmd)->argv[a] = (*cmd)->argv[a + 1];
		(*cmd)->argv_auth[a] = (*cmd)->argv_auth[a + 1];
		a++;
	}
	(*cmd)->argc--;
}

int		is_whitespace(char c)
{
	if (c == ' ' || c == '\n')
		return (1);
	return (0);
}

void	print_step1(char *expr, int *t_ind, int *l_ind)
{
	int i;

	ft_dprintf(2, "-- {red}PARSING{eoc} --\nSTEP 1: {green}replacing{eoc}\n%%");
	i = 0;
	while (expr[i])
	{
		if (is_intouchable(i, t_ind, l_ind))
			ft_dprintf(2, "{red}%c{eoc}", expr[i]);
		else
			ft_dprintf(2, "{green}%c{eoc}", expr[i]);
		i++;
	}
	ft_dprintf(2, "%%\n");
}
