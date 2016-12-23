/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 19:01:35 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/23 18:36:16 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	writor(t_executor **exs, int fd, int *b, int x)
{
	int i;

	if (!exs[0]->ex.r)
		return ;
	i = -1;
	while (exs[0]->ex.r[++i])
	{
		if (exs[0]->ex.r[i]->type == 0 && exs[0]->ex.r[i]->fd_in == fd)
			write(exs[0]->ex.r[i]->fd_out, b, x);
	}
}

void	exec_builtin_redir_err_init(t_executor **exs, t_handle_r *hr,
	t_built_redir *t)
{
	if (hr->p[2].activate && fork() == 0)
	{
		close(hr->p[2].fds[WRITER]);
		while (((t->x) = read(hr->p[2].fds[READER], &(t->b), 50)) > 0)
			writor(exs, 2, (int *)(t->b), (t->x));
		exit(0);
	}
	else if (hr->p[2].activate)
	{
		(t->cpystderr) = dup(STDERR_FILENO);
		close(STDERR_FILENO);
		close(hr->p[2].fds[READER]);
		dup2(hr->p[2].fds[WRITER], STDERR_FILENO);
	}
}

void	exec_builtin_redir_out_init(t_executor **exs, t_handle_r *hr,
	t_built_redir *t)
{
	if (hr->p[1].activate && exs[1] != NULL)
		pipe((t->p[1]).fds);
	if (hr->p[1].activate && fork() == 0)
	{
		if (exs[1] != NULL)
			close((t->p[1]).fds[READER]);
		close(hr->p[1].fds[WRITER]);
		while (((t->x) = read(hr->p[1].fds[READER], &(t->b), 50)) > 0)
		{
			if (exs[1] != NULL)
				write((t->p[1]).fds[WRITER], (t->b), (t->x));
			writor(exs, 1, (int *)(t->b), (t->x));
		}
		exit(0);
	}
	if (hr->p[1].activate)
	{
		if (exs[1] != NULL)
			close((t->p[1]).fds[WRITER]);
		(t->cpystdout) = dup((t->fdout));
		close((t->fdout));
		close(hr->p[1].fds[READER]);
		dup2(hr->p[1].fds[WRITER], (t->fdout));
	}
}

void	exec_builtin(t_executor **exs, t_handle_r *hr, char **env)
{
	t_built_redir t;

	(t.special) = special_redir((*exs)->ex.r);
	(t.fdout) = ((t.special)) ? STDERR_FILENO : STDOUT_FILENO;
	exec_builtin_redir_err_init(exs, hr, &t);
	exec_builtin_redir_out_init(exs, hr, &t);
	(*exs)->ex.fnct((*exs)->av, &g_env, &g_lenv);
	if (hr->p[1].activate)
	{
		close(hr->p[1].fds[WRITER]);
		dup2((t.cpystdout), (t.fdout));
		close((t.cpystdout));
		if (exs[1] != NULL)
		{
			exec_all(&exs[1], env, (t.p[1]).fds[READER]);
			close((t.p[1]).fds[READER]);
		}
	}
	if (hr->p[2].activate)
	{
		close(hr->p[2].fds[WRITER]);
		dup2((t.cpystderr), STDERR_FILENO);
		close((t.cpystderr));
	}
}

t_sf	*read_from_fd(int fd)
{
	t_sf	*tmp;
	t_sf	*curr;
	char	b[WRITING];
	int		len;

	curr = NULL;
	while ((len = read(fd, &b, WRITING)) > 0)
	{
		if (!(tmp = create_packet((char *)b, len)))
		{
			impossibru_error("packet hasn't been created");
			return (NULL);
		}
		if (curr == NULL)
			curr = tmp;
		else
		{
			curr->next = tmp;
			curr = tmp;
		}
	}
	return (curr);
}
