/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/31 13:02:57 by jguthert          #+#    #+#             */
/*   Updated: 2016/06/10 18:06:20 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int			put_history(t_av *av)
{
	int		fd;

	fd = open("/.21sh_history", O_WRONLY, O_CREAT);
	if (fd == -1)
		return (1);
	while ()
	if (close(fd) == -1)
		return (1);
	return (0);
}

int			get_history()
{
	int		fd;
	char	*line;

	fd = open("/.21sh_history", O_RDONLY);
	if (fd == -1)
		return (-1);
	while (get_next_line(fd, &line) > 0)
	{

	}
	if (close(fd) == -1)
		return (1);
	return (0);
}
