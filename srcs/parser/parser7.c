/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/11 15:29:36 by agadhgad          #+#    #+#             */
/*   Updated: 2017/01/24 16:15:58 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void				convert_parse_if1(t_norm_convert_parse *t, t_nparse np)
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

void				convert_parse_if2(t_norm_convert_parse *t, t_nparse np,
			int *t_ind, int *l_ind)
{
	char	*x;

	x = xget_string_l((*t->expr) + np.begin[(t->i)], np.end[(t->i)] -
			np.begin[(t->i)]);
	(t->cmds)[(t->id_cmds)]->argv[(t->cmds)[(t->id_cmds)]->argc] = x;
	(t->cmds)[(t->id_cmds)]->argv_auth[(t->cmds)[(t->id_cmds)]->argc] =
		handle_d(create_np(np, (t->i)), t_ind, l_ind, ft_strlen((*t->expr)));
	(t->cmds)[(t->id_cmds)]->argc++;
	(t->cmds)[(t->id_cmds)]->argv_auth[(t->cmds)[(t->id_cmds)]->argc] = NULL;
	(t->cmds)[(t->id_cmds)]->argv[(t->cmds)[(t->id_cmds)]->argc] = NULL;
}

t_av				**convert_parse(char *expr, t_nparse np, int *t_ind,
		int *l_ind)
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

t_handle_d_param	create_np(t_nparse np, int i)
{
	t_handle_d_param t;

	t.np = np;
	t.i = i;
	return (t);
}
