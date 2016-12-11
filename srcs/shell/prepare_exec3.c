/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/04 18:39:42 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/11 23:11:14 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

t_exec		exec_ret_err(t_exec e, char *msg, char *cmd)
{
	print_err(msg, cmd);
	return (e);
}

char		*get_path(t_list *g_env, t_list *g_lenv)
{
	while (g_env != NULL)
	{
		if (ft_strcmp(((t_env *)g_env->content)->name, "PATH") == 0)
			return (((t_env *)g_env->content)->value);
		g_env = g_env->next;
	}
	while (g_lenv != NULL)
	{
		if (ft_strcmp(((t_env *)g_lenv->content)->name, "PATH") == 0)
			return (((t_env *)g_lenv->content)->value);
		g_lenv = g_lenv->next;
	}
	return (NULL);
}
