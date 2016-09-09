/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:03:36 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/09 16:04:38 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static t_builtin const	g_builtin_list[8] = {
	{"cd", bi_cd},
	{"setenv", bi_setenv},
	{"unsetenv", bi_unsetenv},
	{"env", bi_env},
	{"getenv", bi_getenv},
	{"echo", bi_echo},
	{"exit", bi_exit},
	{"history", bi_history},
};

int			builtin(t_av av, t_list **g_env, t_list **l_env, t_ftl_root *hist)
{
	int		i;

	i = 0;
	if (ft_strcmp(g_builtin_list[7].key, av.cmd) == 0)
		return (g_builtin_list[7].value(av, hist));
	while (i < 7)
	{
		if (ft_strcmp(g_builtin_list[i].key, av.cmd) == 0)
			return(g_builtin_list[i].value(av, g_env, l_env));
		i++;
	}
	return (1);
}
