/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/31 13:02:57 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/31 13:52:47 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int			put_history()
{
	int		fd;

	fd = open("/.21sh_history", O_RDWR, O_CREAT);
	if (fd == -1)
		return (1);
	ft_putstr_fd("tralala", fd);
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
