/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_bin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/13 18:36:16 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/27 14:18:28 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>
#include <stdlib.h>

static char *get_path(t_list *g_env, t_list *l_env)
{
	while (g_env != NULL)
	{
		if (ft_strcmp(((t_env *)g_env->content)->name, "PATH") == 0)
			return (((t_env *)g_env->content)->value);
		g_env = g_env->next;
	}
	while (l_env != NULL)
	{
		if (ft_strcmp(((t_env *)l_env->content)->name, "PATH") == 0)
			return (((t_env *)l_env->content)->value);
		l_env = l_env->next;
	}
	return (NULL);
}

static char **convert_env(t_list *g_env, t_list *l_env)
{
	t_list	*env;
	char	**tab;
	int		i;

	env = g_env;
	if (env == NULL)
		env = l_env;
	i = ft_listlen(env);
	tab = (char **)malloc(sizeof(char *) * (i + 1));
	if (tab == NULL)
		return (NULL);
	tab[i--] = NULL;
	while (env != NULL)
	{
		tab[i--] = ft_strdup(((t_env *)env->content)->str);
		env = env->next;
	}
	return (tab);
}

static char **get_allpath(char *cmd, char *path)
{
	char	**temp;
	char	**allpath;
	int		i;

	i = -1;
	temp = ft_strsplit(path, ':');
	allpath = (char **)malloc(sizeof(char *) * (ft_tablen(temp) + 1));
	while (temp[++i] != NULL)
		allpath[i] = ft_strjoin(temp[i], "/");
	allpath[i] = NULL;
	ft_tabdel(temp);
	temp = (char **)malloc(sizeof(char *) * (ft_tablen(allpath) + 1));
	i = -1;
	while (allpath[++i] != NULL)
		temp[i] = ft_strjoin(allpath[i], cmd);
	temp[i] = NULL;
	ft_tabdel(allpath);
	return (temp);
}

static int	 exec_path(char **arg, char **path, char **env)
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

int		check_bin(t_list *g_env, t_list *l_env, t_av av)
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
		ret = exec_path(av.argv, path, env);
	ft_tabdel(path);
	ft_tabdel(env);
	if (ret == -2)
		return (print_error(INIT_AV("minishell", av.cmd, NULL, 1), 7));
	return (ret);
}
