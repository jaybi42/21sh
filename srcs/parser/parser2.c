/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 20:09:14 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/06 20:09:34 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

t_av **convert_parse(char *expr, t_nparse np, int *t_ind, int *l_ind)
{
	t_av **cmds;
	int id_cmds;
	int i;

	cmds = xmalloc(sizeof(t_av*) * (ft_strlen(expr) + 1));
	id_cmds = 0;
	init_cmd(&cmds[0], ft_strlen(expr));
	cmds[id_cmds]->type = TYPE_NORMAL;
	cmds[id_cmds + 1] = NULL;
	i = 0;
	while (i < np.nb)
	{
		if (np.type[i] > 0)
		{
			init_cmd(&cmds[++id_cmds], ft_strlen(expr));
			cmds[id_cmds]->type = get_type(xget_string_l(expr + np.begin[i], np.end[i] - np.begin[i]));
			if (cmds[id_cmds]->type == -10)
			{
				if (id_cmds > 0)
					cmds[id_cmds-1]->bg = TRUE;
				else
					dprintf(2, "21sh: parse error\n");
				cmds[id_cmds]->type = TYPE_NORMAL;
			}
			cmds[id_cmds + 1] = NULL;
		}
		else
		{
			char *x = xget_string_l(expr + np.begin[i], np.end[i] - np.begin[i]);
			cmds[id_cmds]->argv[cmds[id_cmds]->argc] = x;
			cmds[id_cmds]->argv_auth[cmds[id_cmds]->argc] = handle_d(np, i, t_ind, l_ind, ft_strlen(expr));
			cmds[id_cmds]->argc++;
			cmds[id_cmds]->argv_auth[cmds[id_cmds]->argc] = NULL;
			cmds[id_cmds]->argv[cmds[id_cmds]->argc] = NULL;
		}
		i++;
	}
	cmds[id_cmds + 1] = NULL;
	return (cmds);
}

t_nparse parse(char *expr, int *t_ind, int *l_ind)
{
	t_nparse np;
	int i;
	int i32_tmp;

	nparse_init(&np, expr);
	i = 0;
	while(expr[i] && is_whitespace(expr[i]) && !is_intouchable(i, t_ind, l_ind))
		i++;
	nparse_extend(&np, i);

	while (expr[i] != '\0')
	{
		if (is_intouchable(i, t_ind, l_ind))
		{
			while(expr[i] && is_intouchable(i, t_ind, l_ind))
				i++;
			i--;
		}
		else if (is_whitespace(expr[i]))
		{
			if (!nparse_close(&np, i))
				return (np);
			while(expr[i] && is_whitespace(expr[i]) && !is_intouchable(i, t_ind, l_ind))
				i++;
			if (!is_connector(expr, i, t_ind, l_ind))
				nparse_extend(&np, i);
			i--;
		}
		else if ((i32_tmp = is_connector(expr, i, t_ind,l_ind)))
		{
			if (!nparse_close(&np, (is_intouchable(i-1, t_ind, l_ind) || !is_whitespace(expr[i-1])) ? i : i-1))
				return (np);
			nparse_extend(&np, i);
			np.type[np.nb] = 2;
			if (!nparse_close(&np, i + i32_tmp-1))
				return (np);
			if (is_intouchable(i + i32_tmp, t_ind, l_ind) || !is_whitespace(expr[i + i32_tmp]))
				nparse_extend(&np, i + i32_tmp);
			i += i32_tmp - 1;
		}
		i++;
	}
	if (!nparse_close(&np, i))
		return (np);
	np.nb++;
	if (i > 0 && !is_intouchable(i-1, t_ind, l_ind) && is_whitespace(expr[i - 1]))
		np.nb--;
	np.failed = FALSE;
	return (np);
}
