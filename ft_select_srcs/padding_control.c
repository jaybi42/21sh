/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/16 18:11:42 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/18 18:23:30 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static int			get_maxlen(t_ftl_root *root)
{
	t_ftl_node		*node;
	int				max_len;

	max_len = 0;
	node = root->next;
	while (node != (t_ftl_node *)root)
	{
		if (((t_elem *)node)->name_len > max_len)
			max_len = ((t_elem *)node)->name_len;
		node = node->next;
	}
	return (max_len);
}

int					padding_control(t_ftl_root *root, t_ftl_node *node)
{
	struct winsize	win;
	int				col;
	int				row;
	int				max_len;

	ioctl(0, TIOCGWINSZ, &win);
	win.ws_row -= 1;
	if (win.ws_col > 59)
		win.ws_row -= 10;
	max_len = get_maxlen(root) + 5;
	if (((win.ws_col / max_len) * win.ws_row) < (int)root->size)
		return (1);
	col = ((t_elem *)node)->pos / win.ws_row * max_len;
	row = ((t_elem *)node)->pos % win.ws_row;
	if (win.ws_col > 59)
		row += 10;
	move_termcap("cm", col, row);
	return (0);
}
