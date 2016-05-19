/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/13 15:04:06 by jguthert          #+#    #+#             */
/*   Updated: 2016/01/03 13:39:44 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static t_gnl	*init_gnl(int fd)
{
	t_gnl	*gnl;

	if (!(gnl = malloc(sizeof(*gnl))))
		return (NULL);
	gnl->next = NULL;
	gnl->tmp = ft_strdup("");
	if (gnl->tmp == NULL)
		return (NULL);
	gnl->fd = fd;
	return (gnl);
}

static t_gnl	*retrieve_gnl_for_fd(t_gnl *gnl, int fd)
{
	while (gnl)
	{
		if (gnl->fd == fd)
			return (gnl);
		else if (!gnl->next)
			break ;
		gnl = gnl->next;
	}
	if (gnl == NULL)
		return (NULL);
	else if (!(gnl->next = init_gnl(fd)))
		return (NULL);
	gnl = gnl->next;
	return (gnl);
}

static int		get_ret(t_gnl *gnl, char **line, int ret)
{
	char	*newline;
	char	buff[BUFF_SIZE + 1];
	char	*tmp;

	newline = ft_strchr(gnl->tmp, '\n');
	if (newline == NULL)
	{
		if ((ret = read(gnl->fd, buff, BUFF_SIZE)) < 0)
			return (-1);
		buff[ret] = '\0';
		tmp = ft_strjoin(gnl->tmp, buff);
		if (tmp == NULL)
			return (-1);
		free(gnl->tmp);
		gnl->tmp = tmp;
		return (ret);
	}
	*line = ft_strsub(gnl->tmp, 0, newline - gnl->tmp);
	tmp = ft_strsub(newline, 1, ft_strlen(newline) - 1);
	if (*line == NULL || tmp == NULL)
		return (-1);
	free(gnl->tmp);
	gnl->tmp = tmp;
	return (-2);
}

static int		get_line(t_gnl *gnl, char **line)
{
	int		ret;

	ret = 1;
	while (ret != 0)
	{
		ret = get_ret(gnl, line, ret);
		if (ret == -2)
			return (1);
		if (ret == -1)
			return (-1);
	}
	if (*gnl->tmp == '\0')
		return (0);
	*line = gnl->tmp;
	gnl->tmp = ft_strdup("");
	if (gnl->tmp == NULL)
		return (-1);
	return (1);
}

int				get_next_line(int const fd, char **line)
{
	static t_gnl	*gnl = NULL;
	t_gnl			*fd_gnl;

	if (gnl == NULL)
		gnl = init_gnl(fd);
	fd_gnl = retrieve_gnl_for_fd(gnl, fd);
	if (fd_gnl == NULL || line == NULL)
		return (-1);
	return (get_line(fd_gnl, line));
}
