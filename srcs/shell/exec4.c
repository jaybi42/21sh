/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 19:01:35 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/08 21:03:30 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void		exec_builtin_init(t_executor **exs, int *cpystdout,
		int *cpystderr, t_handle_r *hr)
{
	if (exs[1] != NULL || (hr->is_redirecting && hr->b_out))
	{
		switch_fd_begin(STDOUT_FILENO, cpystdout);
	}
	if (exs[1] != NULL)
		dup_writer_pipe(STDOUT_FILENO, hr->p[1].fds[WRITER]);
	else if (hr->is_redirecting && hr->b_out)
		dup_writer_pipe(STDOUT_FILENO, hr->fdout[WRITER]);
	if (hr->is_redirecting && hr->b_err)
	{
		switch_fd_begin(STDERR_FILENO, cpystderr);
		dup_writer_pipe(STDERR_FILENO, hr->fderr[WRITER]);
	}
}

void		exec_builtin(t_executor **exs, t_handle_r *hr, char **env)
{
	int cpystdout;
	int cpystderr;

	cpystdout = -1;
	cpystderr = -1;
	exec_builtin_init(exs, &cpystdout, &cpystderr, hr);
	(*exs)->ex.fnct((*exs)->av, &g_env, &g_lenv);
	if (exs[1] != NULL || (hr->is_redirecting && hr->b_out))
		switch_fd_end(STDOUT_FILENO, &cpystdout);
	if (hr->is_redirecting && hr->b_err)
		switch_fd_end(STDERR_FILENO, &cpystderr);
	if (exs[1] != NULL)
		exec_all(&exs[1], env, hr->p[1].fds[READER]);
	else if (hr->is_redirecting && hr->b_out)
	{
		close(hr->fdout[WRITER]);
		hr->packets_out = read_from_fd(hr->fdout[READER]);
		close(hr->fdout[READER]);
	}
	if (hr->is_redirecting && hr->b_err)
		close(hr->fderr[WRITER]);
}

t_sf		*read_from_fd(int fd)
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
