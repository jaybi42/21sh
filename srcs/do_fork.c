/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/24 16:57:26 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/16 16:39:42 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>
#include <stdlib.h>

int				do_fork(char *bin, char **all, char **env)
{
	pid_t		ret;
	int			wait_status;

	catch_signal();
	ret = fork();
	if (ret == 0)
		exit(execve(bin, all, env));
	else if (ret == -1)
		return (print_error(INIT_AV(NULL, NULL, NULL, 0), 6));
	else
	{
		ret = waitpid(-1, &wait_status, WUNTRACED);
		if (WIFSIGNALED(wait_status))
			g_prompt.son = 1;
		if (WIFEXITED(wait_status))
			ret = WEXITSTATUS(wait_status);
	}
	return (ret);
}
