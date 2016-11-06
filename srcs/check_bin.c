/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_bin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/13 18:36:16 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/06 17:18:33 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>
#include <stdlib.h>

static int	exec_path2(char **arg, char **path, char **env)
{
	int i;

	i = 0;
	while (path[i] != NULL)
	{
		if (access(path[i], X_OK) != -1)
			return (do_fork(path[i], arg, env));
		i++;
	}
	return (-2);
}

int			check_bin(t_list *g_env, t_list *l_env, t_av av)
{
	char	**env;
	char	**path;
	char	*str;
	int		ret;

	ret = -2;
	env = convert_env(g_env, l_env);
	str = get_path(g_env, l_env);
	path = get_allpath(av.cmd, str);
	if (access(av.cmd, X_OK) != -1)
		ret = do_fork(av.cmd, av.argv, env);
	else
		ret = exec_path2(av.argv, path, env);
	ft_tabdel(path);
	ft_tabdel(env);
	if (ret == -2)
		return (print_error(INIT_AV("minishell", av.cmd, NULL, 1), 7));
	return (ret);
}
