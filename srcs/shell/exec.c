/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 18:18:48 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/23 23:34:36 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			father_handle_redirect(t_handle_r *hr)
{
	if (hr->b_out)
	{
		close(hr->fdout[WRITER]);
		hr->packets_out = read_from_fd(hr->fdout[READER]);
		close(hr->fdout[READER]);
	}
}

int				exec_bin_father(t_executor **exs, t_handle_r *hr, char **env,
								t_solvefive solvefive)
{
	int			ret;
	int			wait_status;
	int			i;

	if (solvefive.fdin != -1)
		close(solvefive.fdin);
	if (hr->p[2].activate)
		redir_err_father(exs, hr);
	if (hr->p[1].activate)
		redir_out_father(exs, hr, env, solvefive.pid);
	i = -1;
	while (++i < 10)
		ft_dprintf(2, "");
	ret = waitpid(solvefive.pid, &wait_status, WCONTINUED);
	ft_dprintf(2, "");
	if (WIFSIGNALED(wait_status))
		g_prompt.son = 1;
	if (WIFEXITED(wait_status))
		ret = WEXITSTATUS(wait_status);
	return (ret);
}

int				exec_all(t_executor **exs, char **env, int fdin)
{
	pid_t					pid;
	int						ret;
	t_handle_r				hr;
	t_solvefive				solvefive;

	ret = 0;
	init_handle_redirect(exs[0]->ex.r, &hr, (exs[1] != NULL) ? TRUE : FALSE);
	pid = -1;
	if ((*exs)->ex.type == BASIC && (pid = fork()) == -1)
		exit(0);
	if (pid == 0)
		exec_bin_child(exs, fdin, &hr, env);
	else if (pid == -1)
		exec_builtin(exs, &hr, env);
	else if ((solvefive.pid = pid) == pid)
	{
		solvefive.fdin = fdin;
		ret = exec_bin_father(exs, &hr, env, solvefive);
	}
	return (ret);
}

void			do_exec_init(t_do_exec *t, t_executor ***exs, int ret)
{
	(t->fdout)[1] = -1;
	(t->env) = convert_env(g_env, g_lenv);
	if (ret != 0)
	{
		pipe((t->fdout));
		(t->i) = 0;
		while ((*exs)[(t->i)])
		{
			if ((*exs)[(t->i) + 1] == NULL)
				get_out(&(*exs)[(t->i)]->ex.r, (t->fdout)[WRITER]);
			(t->i)++;
		}
	}
	(t->o).ret_code = 0;
	(t->o).ret_code = exec_all((*exs), (t->env), -1);
	if (*g_exit != -1)
		clean_exit(*g_exit);
	if (ret != 0)
	{
		close((t->fdout)[WRITER]);
		while (((t->r) = read((t->fdout)[READER], (t->b), WRITING)) > 0)
			x_strjoins(&(t->o).string, (size_t *)&(t->o).len,
					(char *)(t->b), (t->r));
		close((t->fdout)[READER]);
	}
}

t_output		do_exec(t_executor **exs, int ret)
{
	t_do_exec t;

	clear_output(&(t.o));
	(t.o).string[0] = '\0';
	(t.o).ret_code = 0;
	(t.fdout)[0] = -1;
	do_exec_init(&t, &exs, ret);
	g_prompt.son = 0;
	(t.y) = -1;
	while (exs[++(t.y)])
	{
		(t.i) = -1;
		while (exs[(t.y)]->ex.r[++(t.i)])
		{
			if (exs[(t.y)]->ex.r[(t.i)]->type == IGNORE)
				continue ;
			if (ret != 0 && exs[(t.y)]->ex.r[(t.i) + 1] == NULL)
				continue ;
			if (exs[(t.y)]->ex.r[(t.i)]->fd_out != STDOUT_FILENO &&
					exs[(t.y)]->ex.r[(t.i)]->fd_out != STDERR_FILENO &&
					exs[(t.y)]->ex.r[(t.i)]->fd_out != STDIN_FILENO)
				close(exs[(t.y)]->ex.r[(t.i)]->fd_out);
		}
	}
	return ((t.o));
}
