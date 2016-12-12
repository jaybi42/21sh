/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 17:25:47 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/12 17:25:51 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	p_s_a_init(t_norm_simplify *t, char **tmp)
{
	int x;

	x = 0;
	(*tmp) = xmalloc(sizeof(char) * (ft_strlen((*t->expr)) + t->a + 4));
	while ((*t->t_ind)[x] != -1)
	{
		if ((*t->t_ind)[x] >= t->i)
			(*t->t_ind)[x] += t->a + 1;
		x++;
	}
}

void	put_space_around(t_norm_simplify *t)
{
	int		x;
	int		w;
	char	*tmp;
	int		e;

	p_s_a_init(t, &tmp);
	w = 0;
	x = -1;
	while (++x < (int)ft_strlen((*t->expr)))
	{
		if (x < t->i || x > t->i)
			tmp[w++] = (*t->expr)[x];
		else if (x == t->i)
		{
			tmp[w++] = ' ';
			e = -1;
			while (++e < t->a)
				tmp[w++] = (*t->expr)[x + e];
			tmp[w++] = ' ';
			x += e - 1;
		}
	}
	tmp[w] = '\0';
	(*t->expr) = tmp;
	(t->i) += t->a;
}

void	simplify_connector(char **expr, int **t_ind, int **l_ind)
{
	t_norm_simplify t;

	t.expr = expr;
	t.t_ind = t_ind;
	t.l_ind = l_ind;
	t.i = 0;
	while ((*expr)[t.i])
	{
		if ((t.a = is_connector((*expr), t.i, (*t_ind), (*l_ind)))
				&& !is_intouchable(t.i, (*t.t_ind), (*t.l_ind)))
			put_space_around(&t);
		t.i++;
	}
}

t_av	**nparse(char *expr, int *t_ind, int *l_ind)
{
	t_av		**cmds;
	t_nparse	np;

	simplify_connector(&expr, &t_ind, &l_ind);
	simplify_space(&expr, &t_ind, &l_ind);
	np = parse(expr, t_ind, l_ind);
	if (np.failed == TRUE)
	{
		(g_debug) ? ft_dprintf(2,
				"STEP X: {red}stop due to an error{eoc}\n") : 0;
		return (NULL);
	}
	(g_debug) ? ft_dprintf(2,
			"STEP 2: {green}converting{eoc}\n") : 0;
	cmds = convert_parse(expr, np, t_ind, l_ind);
	convert_other(&cmds);
	return (cmds);
}

t_av	**parse_commands(char *expr)
{
	int			*t_ind;
	int			*l_ind;
	t_av		**cmds;
	int			id_cmd;

	(g_debug) ? ft_dprintf(2,
			"{yellow}----------------------{eoc}\n") : 0;
	id_cmd = -1;
	while (is_whitespace(expr[++id_cmd]))
		;
	if ((size_t)id_cmd >= ft_strlen(expr))
		return (NULL);
	if (!(cmds = xmalloc(sizeof(t_av **) * (ft_strlen(expr) + 1))))
		return (NULL);
	id_cmd = 0;
	cmds[id_cmd] = NULL;
	expr = decortique_parse(expr, ft_strlen(expr), &t_ind, &l_ind);
	if (g_debug)
		print_step1(expr, t_ind, l_ind);
	if (!(cmds = nparse(expr, t_ind, l_ind)))
		return (NULL);
	return (updated(cmds));
}
