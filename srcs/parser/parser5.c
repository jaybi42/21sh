/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/07 18:50:10 by ibouchla          #+#    #+#             */
/*   Updated: 2017/01/06 18:30:26 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		in_cmd_is_intouchable(t_av *cmd, int id_argv, int i)
{
	if ((cmd->argv_auth) != NULL)
		if ((cmd->argv_auth[id_argv]) != NULL)
			if ((cmd->argv_auth[id_argv][i]) == 1)
				return (0);
	return (1);
}

void	convert_other(t_av ***cmds)
{
	int i;

	i = 0;
	while ((*cmds)[i])
	{
		get_redirect(&(*cmds)[i]);
		i++;
	}
}

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
