/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/31 13:02:57 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/05 17:55:02 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

static char			*get_history_path(void)
{
	struct stat		stat;
	struct passwd	*pw;
	char			*ans;

	if (lstat(".", &stat) == -1)
		return (NULL);
	pw = getpwuid(stat.st_uid);
	if (pw == NULL)
		return (NULL);
	ans = ft_strjoin(pw->pw_dir, "/.42history");
	return (ans);
}

int					put_history(t_ftl_root *root)
{
	int			fd;
	t_ftl_node	*node;
	char		*path;

	path = get_history_path();
	if (path == NULL)
		return (1);
	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0640);
	ft_strdel(&path);
	if (fd == -1)
		return (1);
	node = (t_ftl_node *)root->next;
	while (node != (t_ftl_node *)root)
	{
		ft_putendl_fd(((t_hist *)node)->str, fd);
		node = node->next;
	}
	if (close(fd) == -1)
		return (1);
	return (0);
}

int					get_history(t_ftl_root *root)
{
	t_hist		hist;
	int			fd;
	char		*line;
	char		*path;

	path = get_history_path();
	if (path == NULL)
		return (1);
	ftl_init(root, sizeof(t_hist));
	fd = open(path, O_RDONLY);
	ft_strdel(&path);
	if (fd == -1)
		return (1);
	ft_bzero(&hist, sizeof(t_hist));
	while (get_next_line(fd, &line) > 0)
	{
		hist.str = line;
		ftl_push_back(root, (FTL_NODE *)(&hist));
	}
	if (close(fd) == -1)
		return (1);
	return (0);
}

int					add_history(char *str, t_ftl_root *root)
{
	t_hist		hist;

	if (str == NULL)
		return (0);
	if (root->next == NULL)
		ftl_init(root, sizeof(t_hist));
	ft_bzero(&hist, sizeof(t_hist));
	hist.str = ft_strdup(str);
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
