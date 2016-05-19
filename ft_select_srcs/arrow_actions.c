/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrow_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/15 14:43:55 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/18 17:50:03 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static int			get_newpos(int pos, int size, int sign)
{
	struct winsize	win;
	int				new_pos;

	ioctl(0, TIOCGWINSZ, &win);
	win.ws_row -= (win.ws_col > 59 ? 11 : 1);
	if (size <= win.ws_row)
		return (pos);
	new_pos = pos + win.ws_row * sign;
	if (new_pos < 0)
		new_pos = pos + size - size % win.ws_row;
	else if (new_pos >= size)
		new_pos = pos - size + size % win.ws_row;
	if (new_pos >= size || new_pos < 0)
		return (pos);
	return (new_pos--);
}

void				up_action(t_ftl_root *root, t_ftl_node **pos)
{
	((t_elem *)*pos)->cursor = 0;
	*pos = (*pos)->prev;
	if (*pos == (t_ftl_node *)root)
		*pos = (*pos)->prev;
	((t_elem *)*pos)->cursor = 1;
}

void				down_action(t_ftl_root *root, t_ftl_node **pos)
{
	((t_elem *)*pos)->cursor = 0;
	*pos = (*pos)->next;
	if (*pos == (t_ftl_node *)root)
		*pos = (*pos)->next;
	((t_elem *)*pos)->cursor = 1;
}

void				right_action(t_ftl_root *root, t_ftl_node **pos)
{
	int		new_pos;

	new_pos = get_newpos(((t_elem *)*pos)->pos, (int)root->size, 1);
	((t_elem *)*pos)->cursor = 0;
	while (new_pos != ((t_elem *)*pos)->pos)
	{
		if (new_pos > ((t_elem *)*pos)->pos)
			*pos = (*pos)->next;
		else
			*pos = (*pos)->prev;
	}
	((t_elem *)*pos)->cursor = 1;
}

void				left_action(t_ftl_root *root, t_ftl_node **pos)
{
	int		new_pos;

	new_pos = get_newpos(((t_elem *)*pos)->pos, (int)root->size, -1);
	((t_elem *)*pos)->cursor = 0;
	while (new_pos != ((t_elem *)*pos)->pos)
	{
		if (new_pos > ((t_elem *)*pos)->pos)
			*pos = (*pos)->next;
		else
			*pos = (*pos)->prev;
	}
	((t_elem *)*pos)->cursor = 1;
}
