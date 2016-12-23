/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 18:18:48 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/18 19:27:38 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			special_redir(t_redirect **r)
{
	int find;
	int i;

	i = -1;
	find = FALSE;
	while (r[++i])
	{
		if (r[i]->fd_in == 2 && r[i]->fd_out == 1)
		{
			find = TRUE;
			break;
		}
	}
	return (find);
}

void		exec_bin_child(t_executor **exs, int fdin,
		t_handle_r *hr, char **env)
{
	int special;

	special = FALSE;
	uncatch_signal();
	son_handle_in(fdin, (*exs)->ex.r);
	if (hr->p[1].activate)
	{
		close(hr->p[1].fds[READER]);
		special = special_redir((*exs)->ex.r);
		dup2(hr->p[1].fds[WRITER], (special == TRUE) ? 2 : 1);
		if (special == TRUE)
			close(1);
		close(hr->p[1].fds[WRITER]);
	}
	if (hr->p[2].activate)
	{
		special = special_redir((*exs)->ex.r);
		if (special == FALSE)
		{
			close(hr->p[2].fds[READER]);
			dup2(hr->p[2].fds[WRITER], 2);
			close(hr->p[2].fds[WRITER]);
		}
	}
	execve((*exs)->ex.path, (*exs)->ex.argv, env);
	exit(0);
}

void read_father(t_executor **exs, t_handle_r *hr, t_pipe p)
{
	int b[50];
	int x;

	while ((x = read(hr->p[1].fds[READER], &b, 50)) > 0)
	{
			if (exs[1] != NULL)
				write(p.fds[WRITER], b, x);
			writor(exs, 1, (int *)b, x);
	}
}

void redir_out_father(t_executor **exs, t_handle_r *hr, t_pipe p, char **env)
{
	int pid;

	close(hr->p[1].fds[WRITER]);
	if (exs[1] != NULL)
			pipe(p.fds);
	if ((pid = fork()) == -1)
		exit(0);
	if (pid == 0)
	{
		if (exs[1] != NULL)
			close(p.fds[READER]);
		read_father(exs, hr, p);
		close(p.fds[WRITER]);
		exit(0);
	}
	else if (exs[1] != NULL)
	{
		close(p.fds[WRITER]);
		waitpid(-1, NULL, WNOHANG);
		exec_all(&exs[1], env, p.fds[READER]);
		close(p.fds[READER]);
		wait(NULL);
	}
}

void redir_err_father(t_executor **exs, t_handle_r *hr)
{
	int b[50];
	int x;
	int pid;

	close(hr->p[2].fds[WRITER]);
	if ((pid = fork()) == -1)
		exit(0);
	else if (pid == 0)
	{
		while ((x = read(hr->p[2].fds[READER], &b, 50)) > 0)
			writor(exs, 2, (int *)b, x);
		exit(0);
	}
}

int			exec_bin_father(t_executor **exs, int fdin,
		t_handle_r *hr, char **env, pid_t mypid)
{
	int ret;
	int wait_status;
	t_pipe p[3];

	if (fdin != -1)
		close(fdin);
	if (hr->p[2].activate)
		redir_err_father(exs, hr);
	if (hr->p[1].activate)
		redir_out_father(exs, hr, p[1], env);
	ret = waitpid(mypid, &wait_status, WUNTRACED);
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
	static struct sigaction sigchld_action = {
		.sa_handler = SIG_DFL,
		.sa_flags = SA_NOCLDWAIT
	};

	init_handle_redirect(exs[0]->ex.r, &hr, (exs[1] != NULL) ? TRUE : FALSE);
	ret = 0;
	pid = -1;
	if ((*exs)->ex.type == BASIC && (pid = fork()) == -1)
		exit(0);
	sigaction(SIGCHLD, &sigchld_action, NULL);
	if (pid == 0)
		exec_bin_child(exs, fdin, &hr, env);
	else if (pid == -1)
		exec_builtin(exs, &hr, env);
	else
		ret = exec_bin_father(exs, fdin, &hr, env, pid);
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
