/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:03:36 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/25 14:55:37 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static t_builtin const	g_builtin_list[7] = {
	{"cd", bi_cd},
	{"setenv", bi_setenv},
	{"unsetenv", bi_unsetenv},
	{"env", bi_env},
	{"getenv", bi_getenv},
	{"echo", bi_echo},
	{"exit", bi_exit},
};

int			builtin(t_av av, t_list **g_env, t_list **l_env)
{
	int		i;
	int		ret;

	ret = 0;
	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(g_builtin_list[i].key, av.cmd) == 0)
		{
			ret = g_builtin_list[i].value(av, g_env, l_env);
			return (ret == -1 ? -1 : 0);
		}
		i++;
	}
	return (1);
}
