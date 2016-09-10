/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:01:52 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/10 14:15:09 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>
#include <sys/wait.h>

/*
** Try the builtin
** if nothing match try to fork, execve
** if -1, exit has been typed
*/

static int		do_shell(t_av av, t_list **g_env, t_list **l_env)
{
	pid_t		ret;

	catch_signal((t_prompt){0, NULL, NULL, 1, NULL});
	ret = fork();
	if (ret == 0)
		do_exec(av, *g_env, *l_env);
	else if (ret == -1)
		return (print_error(av, 6));
	else
		ret = wait(NULL);
	return (ret);
}

int				shell(t_list *av_list, t_list **g_env, t_list **l_env,
					  t_ftl_root *hist)
{
	int			ret;
	t_av		av;

	while (av_list != NULL)
	{
		av = *((t_av *)av_list->content);
		if (av.all == NULL)
			return (0);
		ret = builtin(av, g_env, l_env, hist);
		if (ret == 1)
			ret = do_shell(av, g_env, l_env);
		else if (ret == -1)
			return (1);
		av_list = av_list->next;
	}
	return (0);
}
