/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:01:52 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/13 19:15:57 by jguthert         ###   ########.fr       */
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
			ret = check_bin(av, *g_env, *l_env);
		else if (ret == -1)
			return (1);
		av_list = av_list->next;
	}
	return (0);
}
