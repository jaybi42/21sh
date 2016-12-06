/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:46:38 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/06 19:52:25 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

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

void	init_handle_redirect(t_redirect **redirect, t_handle_r *hr, int ispipe)
{
	int i;

	hr->b_out = 0;
	hr->b_err = 0;
	hr->is_redirecting = 0;
	i = -1;
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
		return ;
	}
	if (!redirect)
		return ;
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
