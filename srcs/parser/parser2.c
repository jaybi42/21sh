/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 20:09:14 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/08 21:03:08 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

typedef struct	s_norm_convert_parse
{
	t_av		**cmds;
	int			id_cmds;
	int			i;
	char		**expr;
}				t_norm_convert_parse;

void		convert_parse_if1(t_norm_convert_parse *t, t_nparse np)
{
	init_cmd(&(t->cmds)[++(t->id_cmds)], ft_strlen((*t->expr)));
	(t->cmds)[(t->id_cmds)]->type = get_type(xget_string_l((*t->expr) +
				np.begin[(t->i)], np.end[(t->i)] - np.begin[(t->i)]));
	if ((t->cmds)[(t->id_cmds)]->type == -10)
	{
		if ((t->id_cmds) > 0)
			(t->cmds)[(t->id_cmds) - 1]->bg = TRUE;
		else
			ft_dprintf(2, "21sh: parse error\n");
		(t->cmds)[(t->id_cmds)]->type = TYPE_NORMAL;
	}
	(t->cmds)[(t->id_cmds) + 1] = NULL;
}

void		convert_parse_if2(t_norm_convert_parse *t, t_nparse np,
			int *t_ind, int *l_ind)
{
	char	*x;

	x = xget_string_l((*t->expr) + np.begin[(t->i)], np.end[(t->i)] -
			np.begin[(t->i)]);
	(t->cmds)[(t->id_cmds)]->argv[(t->cmds)[(t->id_cmds)]->argc] = x;
	(t->cmds)[(t->id_cmds)]->argv_auth[(t->cmds)[(t->id_cmds)]->argc] =
		handle_d(np, (t->i), t_ind, l_ind, ft_strlen((*t->expr)));
	(t->cmds)[(t->id_cmds)]->argc++;
	(t->cmds)[(t->id_cmds)]->argv_auth[(t->cmds)[(t->id_cmds)]->argc] = NULL;
	(t->cmds)[(t->id_cmds)]->argv[(t->cmds)[(t->id_cmds)]->argc] = NULL;
}

t_av		**convert_parse(char *expr, t_nparse np, int *t_ind, int *l_ind)
{
	t_norm_convert_parse	t;

	(t.expr) = &expr;
	(t.cmds) = xmalloc(sizeof(t_av*) * (ft_strlen((*t.expr)) + 1));
	(t.id_cmds) = 0;
	init_cmd(&(t.cmds)[0], ft_strlen((*t.expr)));
	(t.cmds)[(t.id_cmds)]->type = TYPE_NORMAL;
	(t.cmds)[(t.id_cmds) + 1] = NULL;
	(t.i) = 0;
	while ((t.i) < np.nb)
	{
		if (np.type[(t.i)] > 0)
			convert_parse_if1(&t, np);
		else
			convert_parse_if2(&t, np, t_ind, l_ind);
		(t.i)++;
	}
	(t.cmds)[(t.id_cmds) + 1] = NULL;
	return ((t.cmds));
}

typedef struct	s_norm_parse
{
	t_nparse	np;
	int			i;
	int			i32_tmp;
}				t_norm_parse;

int			parse_handle_space(t_norm_parse *p, char *expr,
			int *t_ind, int *l_ind)
{
	if (!nparse_close(&(p->np), (p->i)))
		return (FALSE);
	while (expr[(p->i)] && is_whitespace(expr[(p->i)]) &&
			!is_intouchable((p->i), t_ind, l_ind))
		(p->i)++;
	if (!is_connector(expr, (p->i), t_ind, l_ind))
		nparse_extend(&(p->np), (p->i));
	(p->i)--;
	return (TRUE);
}

int			parse_handle_connector(t_norm_parse *p, char *expr,
			int *t_ind, int *l_ind)
{
	if (!nparse_close(&(p->np), (is_intouchable((p->i) - 1, t_ind, l_ind)
					|| !is_whitespace(expr[(p->i) - 1])) ? (p->i) : (p->i) - 1))
		return (FALSE);
	nparse_extend(&(p->np), (p->i));
	(p->np).type[(p->np).nb] = 2;
	if (!nparse_close(&(p->np), (p->i) + (p->i32_tmp) - 1))
		return (FALSE);
	if (is_intouchable((p->i) + (p->i32_tmp), t_ind, l_ind) ||
			!is_whitespace(expr[(p->i) + (p->i32_tmp)]))
		nparse_extend(&(p->np), (p->i) + (p->i32_tmp));
	(p->i) += (p->i32_tmp) - 1;
	return (TRUE);
}

void		parse_init(t_norm_parse *p, char *expr, int *t_ind, int *l_ind)
{
	nparse_init(&(p->np), expr);
	(p->i) = 0;
	while (expr[(p->i)] && is_whitespace(expr[(p->i)]) &&
			!is_intouchable((p->i), t_ind, l_ind))
		(p->i)++;
	nparse_extend(&(p->np), (p->i));
}

t_nparse ret_parse(t_norm_parse *p, char *expr, int *t_ind, int *l_ind)
{
	if (!nparse_close(&(p->np), (p->i)))
		return ((p->np));
	(p->np).nb++;
	if ((p->i) > 0 && !is_intouchable((p->i) - 1, t_ind, l_ind) &&
			is_whitespace(expr[(p->i) - 1]))
	(p->np).nb--;
	(p->np).failed = FALSE;
	return ((p->np));
}

t_nparse	parse(char *expr, int *t_ind, int *l_ind)
{
	t_norm_parse	p;

	parse_init(&p, expr, t_ind, l_ind);
	while (expr[(p.i)] != '\0')
	{
		if (is_intouchable((p.i), t_ind, l_ind))
		{
			while (expr[(p.i)] && is_intouchable((p.i), t_ind, l_ind))
				(p.i)++;
			(p.i)--;
		}
		else if (is_whitespace(expr[(p.i)]))
		{
			if (!parse_handle_space(&p, expr, t_ind, l_ind))
				return ((p.np));
		}
		else if (((p.i32_tmp) = is_connector(expr, (p.i), t_ind, l_ind)))
		{
			if (!parse_handle_connector(&p, expr, t_ind, l_ind))
				return ((p.np));
		}
		(p.i)++;
	}
	return (ret_parse(&p, expr, t_ind, l_ind));
}
