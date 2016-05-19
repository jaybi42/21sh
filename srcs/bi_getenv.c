/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 20:12:03 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/19 18:00:34 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int			bi_getenv(t_av av, t_list **g_env, t_list **l_env)
{
	t_list		*env;

	env = *g_env;
	(void)l_env;
	if (av.argc == 0)
		return (0);
	while (env != NULL)
	{
		if (ft_strcmp(av.arg[0], ((t_env *)env->content)->name) == 0)
		{
			ft_putendl(((t_env *)env->content)->value);
			break ;
		}
		env = env->next;
	}
	return (0);
}
