#include "21sh.h"

t_exec make_exec_bin(t_av *av, t_list *g_env, t_list *l_env)
{
	t_exec ex;
	char	**env;
	char	**path;
	char	*str;
	int		ret;

	ex.type = -1;
	ex.fnct = NULL;
	env = convert_env(g_env, l_env);
	str = get_path(g_env, l_env);
	path = get_allpath(av->cmd, str);
	if (ft_strstr(av->cmd, "./") != NULL && access(av->cmd, F_OK) != -1)
	{
		if  (access(av->cmd, X_OK) == -1)
		{
			print_err("permission denied", av->cmd);
			return (ex);
		}
		ex.type = BIN;
		ex.path = av->cmd;
		ex.argv = av->argv;
	}
	else
	{
		ret = exec_path(av->cmd, path);
		if (ret != -1)
		{
			ex.type = BIN;
			ex.path = path[ret];
			ex.argv = av->argv;
		}
		else
		{
			print_err("command not found", av->cmd);
		}
	}
	return (ex);
}

/*
 ** Try the builtin
 ** if nothing match try to fork, execve
 ** if -1, exit has been typed
 */
void make_r(t_redirect ***r)
{
	int i;

	i = 0;
	while ((*r)[i])
	{
		if ((*r)[i]->type == 0 && (*r)[i]->fd_out == -1 && (*r)[i]->path != NULL)
		{
			(*r)[i]->fd_out = open((*r)[i]->path, (*r)[i]->open_flag, 0666);
			(g_debug) ? ft_dprintf(2, "[?] we did an open(\"%s\")", (*r)[i]->path) : 0;
		}
		else if ((*r)[i]->type == 1 && (*r)[i]->fd == -1 && (*r)[i]->path != NULL)
		{
			(*r)[i]->fd = open((*r)[i]->path, (*r)[i]->open_flag, 0666);
			(g_debug) ? ft_dprintf(2, "[?] we did an open(\"%s\")", (*r)[i]->path) : 0;
		}
		i++;
	}
}

t_exec make_exec(t_av *av, t_list **g_env, t_list **l_env)
{
	t_exec ex;
	ex = make_exec_builtin(av);
	if (ex.type == -1)
	{
		ex = make_exec_bin(av, *g_env,*l_env);
	}
	ex.r = av->redirect;
	//handle_red
	make_r(&ex.r);
	return (ex);
}
