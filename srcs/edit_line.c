/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 12:20:51 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/18 17:48:32 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <unistd.h>

static void		give_list(t_ftl_root *root)
{
	t_ftl_node	*node;
	bool		first;

	node = root->next;
	first = 1;
	while (node != (t_ftl_node *)root)
	{
		if (((t_elem *)node)->selected == 1)
		{
			if (first == 1)
				first = 0;
			else
				ft_putchar_fd(' ', 1);
			ft_putstr_fd(((t_elem *)node)->name, 1);
		}
		node = node->next;
	}
}

static int		do_select(t_ftl_root *root)
{
	char		buf[5];
	t_ftl_node	*pos;

	pos = root->next;
	while (1)
	{
		if (print_select(root) == 1)
			continue ;
		ft_bzero(&buf, 5);
		if (read(0, buf, 5) == -1)
			return (1);
		if (buf[0] == 27 && buf[1] == 0)
			return (0);
		if (buf[0] == 10 && buf[1] == 0)
			return (2);
		if (actions(buf, root, &pos) == 1)
			return (1);
		if (pos == (t_ftl_node *)root)
			return (0);
	}
}

int				ft_select(char **av, int ac)
{
	t_ftl_root	root;
	int			ret;

	ftl_init(&root, sizeof(t_elem));
	if (init_list(&root, av, ac) == 1)
		return (print_error("ft_select", 6));
	catch_sig(&root);
	if (init_term() == 1)
		return (1);
	ret = do_select(&root);
	reset_term();
	if (ret == 2)
		give_list(&root);
	return (0);
}
