/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/15 16:48:27 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/06 18:15:30 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>

static t_env const	g_initenv[6] = {
	{"PATH", NULL, "PATH="},
	{"HOME", NULL, "HOME="},
	{"USER", NULL, "USER="},
	{"LOGNAME", NULL, "LOGNAME="},
	{"PWD", NULL, "PWD="},
	{"SHELL", NULL, "SHELL="},
};

static char			**get_info(void)
{
	struct stat		stat;
	struct passwd	*pw;
	char			**t;
	char			*tmp;

	if (lstat(".", &stat) == -1 || (pw = getpwuid(stat.st_uid)) == NULL)
		return (NULL);
	if ((t = (char **)malloc(sizeof(char *) * 7)) == NULL)
		return (NULL);
	t[6] = NULL;
	t[0] = ft_strdup("");
	t[1] = ft_strdup(pw->pw_dir);
	t[2] = ft_strdup(pw->pw_name);
	t[3] = ft_strdup(pw->pw_name);
	t[5] = ft_strdup(pw->pw_shell);
	tmp = getwd(NULL);
	if (tmp == NULL)
		return (NULL);
	t[4] = ft_strdup(tmp);
	ft_strdel(&tmp);
	if (!t[0] || !t[1] || !t[2] || !t[3] || !t[4] || !t[5])
		return (NULL);
	return (t);
}

static int			set_lenv(t_list **e)
{
	char			**t;
	t_env			env;
	t_list			*tamp;
	int				i;

	i = 0;
	t = get_info();
	if (t == NULL)
		return (1);
	while (i < 6)
	{
		env.name = ft_strdup((char *)g_initenv[i].name);
		env.value = t[i];
		env.str = ft_strjoin((char *)g_initenv[i].str, t[i]);
		tamp = ft_lstnew((void *)&env, sizeof(env));
		if (tamp == NULL)
			return (1);
		ft_lstadd_last(e, tamp);
		i++;
	}
	free(t);
	t = NULL;
	return (0);
}

static int			set_genv(t_list **e, char **environ)
{
	t_env			env;
	t_list			*tamp;
	int				i;
	int				j;

	i = 0;
	while (environ[i] != NULL)
	{
		env.str = ft_strdup(environ[i]);
		j = 0;
		while (environ[i][j] != '=' && environ[i][j] != '\0')
			j++;
		env.name = ft_strsub(environ[i], 0, j);
		if (environ[i][j] != '\0' && environ[i][++j] != '\0')
			env.value = ft_strsub(environ[i], j, ft_strlen(environ[i]) - j);
		else
			env.value = NULL;
		tamp = ft_lstnew((void *)&env, sizeof(env));
		if (tamp == NULL)
			return (1);
		ft_lstadd_last(e, tamp);
		i++;
	}
	return (0);
}

static void			set_shlvl(t_list *g_env)
{
	char			*new_value;
	char			*temp;
	t_env			*env;

	while (g_env != NULL)
	{
		env = (t_env *)g_env->content;
		if (ft_strcmp(env->name, "SHLVL") == 0 && env->value != NULL)
		{
			new_value = ft_itoa((ft_atoi(env->value) + 1));
			ft_strdel(&env->value);
			env->value = new_value;
			temp = ft_strjoin(env->name, "=");
			if (temp != NULL)
			{
				if (env->str != NULL)
					ft_strdel(&env->str);
				env->str = ft_strjoin(temp, new_value);
				ft_strdel(&temp);
			}
			break ;
		}
		g_env = g_env->next;
	}
}

int					init_env(t_list **g_env, t_list **g_lenv)
{
	extern char		**environ;

	*g_lenv = NULL;
	*g_env = NULL;
	if (set_lenv(g_lenv) == 1)
		return (1);
	if (*environ != NULL)
	{
		if (set_genv(g_env, environ) == 1)
			return (1);
		set_shlvl(*g_env);
	}
	return (0);
}
