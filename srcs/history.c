/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/31 13:02:57 by jguthert          #+#    #+#             */
/*   Updated: 2016/06/14 18:09:12 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <fcntl.h>
#include <unistd.h>

int				put_history(t_ftl_root *root)
{
	int			fd;
	t_ftl_node	*node;

	fd = open("~/.21sh_history", O_CREAT | O_WRONLY);
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

	fd = open("~/.21sh_history", O_RDONLY);
	if (fd == -1)
		return (ERROR);
	ft_bzero(&hist, sizeof(t_hist));
	while (get_next_line(fd, &line) > 0)
	{
		hist.str = line;
		ftl_push_front(root, (FTL_NODE *)(&hist));
	}
	if (close(fd) == -1)
		return (ERROR);
	return (0);
}

static void fill_root(t_ftl_root *root, char *str1, char *str2)
{
	t_hist	hist;

	ft_bzero(&hist, (sizeof(t_hist)));
	hist.str = ft_strdup(str1);
	ftl_push_front(root, (FTL_NODE *)(&hist));
	hist.str = ft_strdup(str2);
	ftl_push_front(root, (FTL_NODE *)(&hist));
}

static void print_history(t_ftl_root *root)
{
	t_ftl_node	*node;

	node = (t_ftl_node *)root->next;
	while (node != (t_ftl_node *)root)
	{
		ft_putendl_fd(((t_hist *)node)->str, 2);
		node = node->next;
	}
}

void			history(void)
{
	t_ftl_root	root1;
	t_ftl_root	root2;
	char		str1[] = "je\ntest\nquatres\nlignes";
	char		str2[] = "jetestuneligne";

	ftl_init(&root1, sizeof(t_elem));
	ftl_init(&root2, sizeof(t_elem));
	fill_root(&root1, str1, str2);
//	fill_root(&root1, str1, str2);
	put_history(&root1);
	get_history(&root2);
	print_history(&root1);
	print_history(&root2);
	sleep(2);
}
