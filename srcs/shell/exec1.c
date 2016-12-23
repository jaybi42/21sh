/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/23 17:49:39 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/23 19:14:49 by malaine          ###   ########.fr       */
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
			break ;
		}
	}
	return (find);
}

void		exec_bin_child(t_executor **exs, int fdin,
		t_handle_r *hr, char **env)
{
	int special;

	uncatch_signal();
	son_handle_in(fdin, (*exs)->ex.r);
	if (((special = FALSE) == FALSE) && hr->p[1].activate)
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

void		read_father(t_executor **exs, t_handle_r *hr, t_pipe p)
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

void		redir_out_father(t_executor **exs, t_handle_r *hr,
							t_pipe p, char **env)
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

void		redir_err_father(t_executor **exs, t_handle_r *hr)
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
