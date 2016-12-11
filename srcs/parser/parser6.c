/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:52:23 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/08 21:04:55 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	simplify_space(char **expr, int **t_ind, int **l_ind)
{
	t_norm_simplify t;

	t.expr = expr;
	t.t_ind = t_ind;
	t.l_ind = l_ind;
	t.i = 0;
	while ((*expr)[t.i])
	{
		if (is_whitespace((*expr)[t.i]) &&
				!is_intouchable(t.i, (*t.t_ind), (*t.l_ind)))
		{
			t.a = 0;
			while (is_whitespace((*expr)[t.i + t.a])
					&& !is_intouchable(t.i + t.a,
						(*t_ind), (*l_ind)))
				t.a++;
			t.a--;
			if (t.a >= 1)
				exclude_s(&t);
		}
		t.i++;
	}
}

void	exclude_s(t_norm_simplify *t)
{
	int		x;
	int		w;
	char	*tmp;

	x = 0;
	tmp = xmalloc(sizeof(char *) * (ft_strlen((*t->expr)) + 1));
	while ((*t->t_ind)[x] != -1)
	{
		if ((*t->t_ind)[x] >= t->i + t->a)
			(*t->t_ind)[x] -= t->a;
		x++;
	}
	x = -1;
	w = 0;
	while (++x < (int)ft_strlen((*t->expr)))
		if (x < t->i || (!(x >= t->i && x < t->i + t->a)))
			tmp[w++] = (*t->expr)[x];
	tmp[w] = '\0';
	(*t->expr) = tmp;
}

void	update_debug(t_av **av, int i)
{
	int x;
	int b;

	x = -1;
	while (av[i]->argv[++x])
	{
		ft_dprintf(2, "cmd %.*s %d argv[%d] : %%", (i + 1) * 2,
				"---------------------------------------", i, x);
		b = -1;
		while (av[i]->argv[x][++b])
		{
			if (av[i]->argv_auth[x][b] == 1)
				ft_dprintf(2, "{green}%c{eoc}", av[i]->argv[x][b]);
			else
				ft_dprintf(2, "{red}%c{eoc}", av[i]->argv[x][b]);
		}
		ft_dprintf(2, "%%\n");
	}
}

void	update_arg(t_av ***av, int i)
{
	if ((*av)[i]->argv[0] != NULL)
	{
		(*av)[i]->cmd = (*av)[i]->argv[0];
		(*av)[i]->arg = copy_array_begin(1, (*av)[i]->argv);
		(*av)[i]->argc--;
	}
	else
	{
		(*av)[i]->cmd = NULL;
		(*av)[i]->arg = NULL;
	}
}

t_av	**updated(t_av **av)
{
	int i;

	i = 0;
	while (av[i] != NULL)
	{
		if (av[i]->argv != NULL)
			update_arg(&av, i);
		else
		{
			av[i]->cmd = NULL;
			av[i]->arg = NULL;
		}
		(g_debug) ? update_debug(av, i) : 0;
		i++;
	}
	(g_debug) ? ft_dprintf(2, "{yellow}----------------------{eoc}\n") : 0;
	return (av);
}
