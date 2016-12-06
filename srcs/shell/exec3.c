/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 18:16:19 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/06 18:16:45 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void	impossibru_error(char *s)
{
	ft_dprintf(2, "%s: ERR!: %s\n", NAME, s);
}

void	son_handle_redirect(t_handle_r *hr)
{
	if (hr->b_out)
	{
		close(hr->fdout[READER]);
		dup2(hr->fdout[WRITER], 1);
		close(hr->fdout[WRITER]);
	}
	if (hr->b_err)
	{
		close(hr->fderr[READER]);
		dup2(hr->fderr[WRITER], 1);
		close(hr->fderr[WRITER]);
	}
}

void	switch_fd_begin(int fd, int *new_fd)
{
	(*new_fd) = dup(fd);
	close(fd);
}

void	switch_fd_end(int fd, int *old_fd)
{
	dup2((*old_fd), fd);
	close((*old_fd));
}

void	dup_writer_pipe(int fd, int writer)
{
	dup2(writer, fd);
	close(writer);
}
