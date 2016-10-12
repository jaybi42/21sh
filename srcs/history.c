/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/31 13:02:57 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/12 13:10:08 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <fcntl.h>
#include <unistd.h>

int				put_history(t_ftl_root *root)
{
	int			fd;
	t_ftl_node	*node;

	fd = open("history", O_CREAT | O_WRONLY | O_TRUNC, 0640);
	if (fd == -1)
		return (ERROR);
	node = (t_ftl_node *)root->next;
	while (node != (t_ftl_node *)root)
	{
		ft_putendl_fd(((t_hist *)node)->str, fd);
		node = node->next;
	}
	if (close(fd) == -1)
		return (ERROR);
	return (0);
}

int				get_history(t_ftl_root *root)
{
	t_hist		hist;
	int			fd;
	char		*line;

	ftl_init(root, sizeof(t_hist));
	fd = open("history", O_RDONLY);
	if (fd == -1)
		return (1);
	ft_bzero(&hist, sizeof(t_hist));
	while (get_next_line(fd, &line) > 0)
	{
		hist.str = line;
		ftl_push_back(root, (FTL_NODE *)(&hist));
	}
	if (close(fd) == -1)
		return (ERROR);
	return (0);
}

int				add_history(char *str, t_ftl_root *root)
{
	t_hist		hist;

	if (str == NULL)
		return (0);
	if (root->next == NULL)
		ftl_init(root, sizeof(t_hist));
	ft_bzero(&hist, sizeof(t_hist));
	hist.str = str;
	while (1)
	{
		if (*str == '\0')
			return (0);
		else if (ft_isspace(*str) == 0)
			break ;
		str++;
	}
	return (ftl_push_back(root, (FTL_NODE *)&hist));
}
