/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:01:52 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/23 18:39:23 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>

static int		do_shell(t_av av, t_list **g_env, t_list **l_env)
{
	pid_t		ret;

	catch_signal(0, NULL, NULL, 1);
	ret = fork();
	if (ret == 0)
		do_exec(av, *g_env, *l_env);
	else if (ret == -1)
		return (print_error(av, 6));
	else
		ret = wait(NULL);
	return (ret);
}

int				shell(t_list *av_list, t_list **g_env, t_list **l_env)
{
	int			ret;
	t_av		av;

	while (av_list != NULL)
	{
		av = *((t_av *)av_list->content);
		if (av.all == NULL)
			return (0);
		ret = builtin(av, g_env, l_env);
		if (ret == 1)
			ret = do_shell(av, g_env, l_env);
		av_list = av_list->next;
	}
	return (0);
}
