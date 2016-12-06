/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 18:11:28 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/06 18:14:06 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static t_builtin const g_builtin_list[NB_BUILT] = {
	{"cd", bi_cd},
	{"setenv", bi_setenv},
	{"unsetenv", bi_unsetenv},
	{"env", bi_env},
	{"getenv", bi_getenv},
	{"echo", bi_echo},
	{"exit", bi_exit},
	{"clear", bi_clear},
	{"history", bi_history},
	{"alias", bi_alias},
	{"42info", bi_42info},
	{"glob", bi_glob}
};

int			get_out(t_redirect ***r, int fd_out)
{
	int i;

	i = 0;
	while ((*r)[i])
		i++;
	(*r)[i] = xmalloc(sizeof(t_redirect));
	(*r)[i]->type = 0;
	(*r)[i]->fd_in = 1;
	(*r)[i]->fd_out = fd_out;
	(*r)[i + 1] = NULL;
	return (1);
}

char		**convert_env(t_list *g_env, t_list *l_env)
{
	t_list	*env;
	char	**t;
	int		i;

	env = g_env;
	if (env == NULL)
		env = l_env;
	i = ft_listlen(env);
	t = (char **)xmalloc(sizeof(char *) * (i + 1));
	if (t == NULL)
		return (NULL);
	t[i--] = NULL;
	while (env != NULL)
	{
		t[i--] = x_strdup(((t_env *)env->content)->str);
		env = env->next;
	}
	return (t);
}

char		**get_allpath(char *cmd, char *path)
{
	char	**temp;
	char	**temp2;
	char	**allpath;
	int		i;
	int		a;

	i = -1;
	temp = x_strsplit(path, ':');
	allpath = (char **)xmalloc(sizeof(char *) * (ft_tablen(temp) + 1));
	while (temp[++i] != NULL)
		allpath[i] = x_strjoin(temp[i], "/");
	allpath[i] = NULL;
	temp2 = (char **)xmalloc(sizeof(char *) * (ft_tablen(allpath) + 2));
	i = -1;
	a = -1;
	while (allpath[++a])
		if (ft_strncmp(cmd, allpath[a], ft_strlen(allpath[a])) == 0)
		{
			temp2[++i] = cmd;
			break ;
		}
	while (allpath[++i] != NULL)
		temp2[i] = x_strjoin(allpath[i], cmd);
	temp2[i] = NULL;
	return (temp2);
}

int			exec_path(char *cmd, char **path)
{
	int i;

	i = 0;
	while (path[i] != NULL)
	{
		if (access(path[i], X_OK) != -1)
			return (i);
		if (access(path[i], F_OK) != -1 && access(path[i], X_OK) == -1)
		{
			print_err("permission denied", cmd);
			return (-1);
		}
		i++;
	}
	return (-1);
}

t_exec		make_exec_builtin(t_av *av)
{
	t_exec	ex;
	int		i;

	i = 0;
	ex.type = -1;
	while (i < NB_BUILT)
	{
		if (ft_strcmp(g_builtin_list[i].key, av->cmd) == 0)
		{
			ex.fnct = g_builtin_list[i].value;
			ex.argv = av->argv;
			ex.path = NULL;
			ex.type = BUILTIN;
			break ;
		}
		i++;
	}
	return (ex);
}
