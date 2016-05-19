/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod_actions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/15 14:52:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/18 17:49:01 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static void		realloc_pos(t_ftl_root *root, t_ftl_node *pos)
{
	while (pos != (t_ftl_node *)root)
	{
		((t_elem *)pos)->pos--;
		pos = pos->next;
	}
}

void			del_action(t_ftl_root *root, t_ftl_node **pos)
{
	t_ftl_node	*node;

	node = (*pos)->next;
	ftl_erase_pos(root, *pos, NULL);
	*pos = node;
	realloc_pos(root, *pos);
	if (*pos == (t_ftl_node *)root)
		*pos = (*pos)->next;
	((t_elem *)*pos)->cursor = 1;
}

void			sel_action(t_ftl_root *root, t_ftl_node **pos)
{
	((t_elem *)*pos)->selected = !(((t_elem *)*pos)->selected);
	if ((((t_elem *)*pos)->selected) == 1)
	{
		((t_elem *)*pos)->cursor = 0;
		*pos = (*pos)->next;
		if (*pos == (t_ftl_node *)root)
			*pos = (*pos)->next;
		((t_elem *)*pos)->cursor = 1;
	}
}
