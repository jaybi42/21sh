/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/24 16:57:26 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/14 14:35:57 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>
#include <stdlib.h>

int				do_fork(char *bin, char **all, char **env)
{
	pid_t		ret;
	int			wait_status;

	catch_signal((t_prompt){0, NULL, NULL, 1, NULL});
	ret = fork();
	if (ret == 0)
		exit(execve(bin, all, env));
	else if (ret == -1)
		return (print_error(INIT_AV(NULL, NULL, NULL, 0), 6));
	else
	{
		ret = waitpid(-1, &wait_status, WUNTRACED);
		if (WIFEXITED(wait_status))
			ret = WEXITSTATUS(wait_status);
	}
	return (ret);
}

/*
int			do_exec(t_av av, t_list *g_env, t_list *l_env)
{
	char	**env;
	char	**path;
	char	*str;

	env = convert_env(g_env, l_env);
	str = get_path(g_env, l_env);
	if (access(av.cmd, X_OK) != -1)
		execve(av.cmd, av.all, env);
	path = get_allpath(av.cmd, str);
	exec_path(av.all, path, env);
	print_error(INIT_AV("minishell", av.cmd, NULL, 1), 7);
	ft_tabdel(path);
	ft_tabdel(env);
	return (bi_exit(INIT_AV(NULL, NULL, NULL, 1), NULL, NULL));
}
*/
