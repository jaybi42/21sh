/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 18:18:48 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/15 13:22:39 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		exec_bin_child(t_executor **exs, int fdin,
		t_handle_r *hr, char **env)
{
	son_handle_in(fdin, (*exs)->ex.r);
	if (hr->p[1].activate == 1)
	{
		close(hr->p[1].fds[READER]);
		dup2(hr->p[1].fds[WRITER], 1);
		close(hr->p[1].fds[WRITER]);
	}
	if (hr->is_redirecting)
		son_handle_redirect(hr);
	execve((*exs)->ex.path, (*exs)->ex.argv, env);
	exit(0);
}

int			exec_bin_father(t_executor **exs, int fdin,
		t_handle_r *hr, char **env)
{
	int ret;
	int wait_status;

	if (fdin != -1)
		close(fdin);
	if (hr->is_redirecting)
		father_handle_redirect(hr);
	if (exs[1] != NULL)
	{
		close(hr->p[1].fds[WRITER]);
		exec_all(&exs[1], env, hr->p[1].fds[READER]);
	}
	ret = waitpid(-1, &wait_status, WUNTRACED);
	if (WIFSIGNALED(wait_status))
		g_prompt.son = 1;
	if (WIFEXITED(wait_status))
		ret = WEXITSTATUS(wait_status);
	return (ret);
}

int			exec_all(t_executor **exs, char **env, int fdin)
{
	pid_t		pid;
	int			ret;
	t_handle_r	hr;

	init_handle_redirect(exs[0]->ex.r, &hr, (exs[1] != NULL) ? TRUE : FALSE);
	ret = 0;
	pid = -1;
	if ((*exs)->ex.type == BASIC && (pid = fork()) == -1)
		exit(0);
	if (pid == 0)
		exec_bin_child(exs, fdin, &hr, env);
	else if (pid == -1)
		exec_builtin(exs, &hr, env);
	else
		ret = exec_bin_father(exs, fdin, &hr, env);
	if (hr.is_redirecting)
		active_redirect((*exs)->ex.r, &hr);
	return (ret);
}

void		do_exec_init(t_do_exec *t, t_executor ***exs, int ret)
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

t_output	do_exec(t_executor **exs, int ret)
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
