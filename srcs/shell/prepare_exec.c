/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 17:47:21 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/08 21:09:23 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void		grate_une_ligne(int *type, void **fnct)
{
	(*type) = -1;
	(*fnct) = NULL;
}

t_exec		get_abs_path(char *cmd, char **argv)
{
	char	**env;
	char	**path;
	char	*str;
	t_exec	ex;
	int		ret;

	grate_une_ligne(&ex.type, (void *)&ex.fnct);
	if (!g_hash)
	{
		env = convert_env(g_env, l_env);
		str = get_path(g_env, l_env);
		path = get_allpath(cmd, str);
		ret = exec_path(cmd, path);
	}
	else
		str = get_hash_path(&g_hash, cmd);
	if ((!g_hash && ret != -1) || (g_hash && str))
	{
		ex.type = BASIC;
		ex.path = (!g_hash) ? path[ret] : str;
		ex.argv = argv;
	}
	else
		print_err("command not found", cmd);
	return (ex);
}

t_exec		make_exec_bin(t_av *av, t_list *g_env, t_list *l_env)
{
	t_exec	ex;

	ex.type = -1;
	ex.fnct = NULL;
	(void)g_env;
	(void)l_env;
	if (ft_strstr(av->cmd, "./") != NULL && access(av->cmd, F_OK) != -1)
	{
		if (access(av->cmd, X_OK) == -1)
		{
			print_err("permission denied", av->cmd);
			return (ex);
		}
		ex.type = BASIC;
		ex.path = av->cmd;
		ex.argv = av->argv;
	}
	else
		return (get_abs_path(av->cmd, av->argv));
	return (ex);
}

void		make_r(t_redirect ***r)
{
	int i;

	i = 0;
	while ((*r)[i])
	{
		if ((*r)[i]->type == 0 && (*r)[i]->fd_out == -1
				&& (*r)[i]->path != NULL)
		{
			(*r)[i]->fd_out = open((*r)[i]->path,
					(*r)[i]->open_flag, 0666);
			(g_debug) ? ft_dprintf(2, "[?] we did an open(\"%s\")",
					(*r)[i]->path) : 0;
		}
		else if ((*r)[i]->type == 1 && (*r)[i]->fd == -1
				&& (*r)[i]->path != NULL)
		{
			(*r)[i]->fd = open((*r)[i]->path,
					(*r)[i]->open_flag, 0666);
			(g_debug) ? ft_dprintf(2, "[?] we did an open(\"%s\")",
					(*r)[i]->path) : 0;
		}
		i++;
	}
}

t_exec		make_exec(t_av *av, t_list **g_env, t_list **l_env)
{
	t_exec ex;

	ex = make_exec_builtin(av);
	if (ex.type == -1)
		ex = make_exec_bin(av, *g_env, *l_env);
	ex.r = av->redirect;
	make_r(&ex.r);
	return (ex);
}
