/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:46:38 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/08 19:03:19 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void	father_handle_redirect(t_handle_r *hr)
{
	if (hr->b_out)
	{
		close(hr->fdout[WRITER]);
		hr->packets_out = read_from_fd(hr->fdout[READER]);
		close(hr->fdout[READER]);
	}
}

void	active_redirect(t_redirect **r, t_handle_r *hr)
{
	t_sf	*p;
	int		i;

	i = -1;
	while (r[++i])
	{
		if (r[i]->type == IGNORE)
			continue ;
		if (r[i]->fd_in == STDOUT_FILENO)
		{
			p = hr->packets_out;
			while (p)
			{
				write(r[i]->fd_out, p->s, p->len);
				p = p->next;
			}
		}
	}
}

void	son_handle_in(int fdin, t_redirect **r)
{
	int	i;

	if (fdin != -1)
	{
		dup2(fdin, STDIN_FILENO);
		close(fdin);
	}
	else if (r != NULL)
	{
		i = -1;
		while (r[++i])
		{
			if (r[i]->type == IGNORE)
			{
				dup2(r[i]->fd, STDIN_FILENO);
				close(r[i]->fd);
				break ;
			}
		}
	}
}

int		init_handle_redirect2(t_redirect **redirect,
		t_handle_r *hr, int ispipe, int i)
{
	while (++i < 3)
	{
		hr->p[i].fds[0] = -1;
		hr->p[i].fds[1] = -1;
		hr->p[i].activate = 0;
	}
	if (ispipe)
	{
		hr->p[1].activate = 1;
		pipe(hr->p[1].fds);
		return (FALSE);
	}
	if (!redirect)
		return (FALSE);
	i = -1;
	while (redirect[++i])
	{
		if (redirect[i]->type == 1)
			continue ;
		if (redirect[i]->fd_in == 1)
			hr->b_out = 1;
		if (redirect[i]->fd_in == 2)
			hr->b_err = 1;
	}
	return (TRUE);
}

void	init_handle_redirect(t_redirect **redirect, t_handle_r *hr, int ispipe)
{
	hr->b_out = 0;
	hr->b_err = 0;
	hr->is_redirecting = 0;
	if (!init_handle_redirect2(redirect, hr, ispipe, -1))
		return ;
	if (hr->b_out)
	{
		if (pipe(hr->fdout) == -1)
			dprintf(2, "failed to pipe\n");
		hr->packets_out = NULL;
	}
	if (hr->b_err)
	{
		pipe(hr->fderr);
		hr->packets_err = NULL;
	}
	hr->is_redirecting = (hr->b_out || hr->b_err) ? 1 : 0;
}
