/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:03:36 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/24 20:21:12 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define NB_BUILTIN 14

static t_builtin const	g_builtin_list[NB_BUILTIN] = {
	{"cd", bi_cd},
	{"setenv", bi_setenv},
	{"unsetenv", bi_unsetenv},
	{"unset", bi_unset},
	{"env", bi_env},
	{"getenv", bi_getenv},
	{"echo", bi_echo},
	{"exit", bi_exit},
	{"export", bi_export},
	{"clear", bi_clear},
	{"history", bi_history},
	{"alias", bi_alias},
	{"42info", bi_42info},
	{"glob", bi_glob}
};

int			builtin(t_list **g_env, t_list **g_lenv, t_ftl_root *hist, t_av *av)
{
	int i;

	i = 0;
	if (ft_strcmp(g_builtin_list[NB_BUILTIN].key, av->cmd) == 0)
	{
		g_builtin_list[NB_BUILTIN].value((*av), hist);
		return (0);
	}
	while (i < NB_BUILTIN)
	{
		if (ft_strcmp(g_builtin_list[i].key, av->cmd) == 0)
			return (g_builtin_list[i].value((*av), g_env, g_lenv) & -1);
		i++;
	}
	return (1);
}
