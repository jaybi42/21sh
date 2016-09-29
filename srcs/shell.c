/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:01:52 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/27 16:50:52 by agadhgad         ###   ########.fr       */
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

int				shell(t_av **av, t_list **g_env, t_list **l_env, t_ftl_root *hist)
{
	int i;
	int ret;

	i = -1;
	while (av[++i] != NULL)
	{
		if (av[i]->argv[0] == NULL)
			continue;
		if ((ret = builtin(g_env, l_env, hist, av[i])) == 1)
			ret = check_bin(*g_env, *l_env, (*av)[i]);
		else if (ret == -1)
			return (1);
	}
	return (0);
}
