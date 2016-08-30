/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftl_insert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 16:28:27 by jguthert          #+#    #+#             */
/*   Updated: 2016/08/23 12:58:05 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>

int				ftl_push_back(t_ftl_root *l, FTL_NODEC *node)
{
	t_ftl_node	*temp;

	temp = ft_memdup(node, l->node_size);
	if (temp == NULL)
		return (ENOMEM);
	temp->next = (FTL_NODE *)l;
	temp->prev = l->prev;
	l->prev->next = temp;
	l->prev = temp;
	l->size++;
	return (0);
}

int				ftl_push_front(t_ftl_root *l, FTL_NODEC *node)
{
	t_ftl_node	*temp;

	temp = ft_memdup(node, l->node_size);
	if (temp == NULL)
		return (ENOMEM);
	temp->next = l->next;
	temp->prev = (FTL_NODE *)l;
	l->next->prev = temp;
	l->next = temp;
	l->size++;
	return (0);
}

int				ftl_insert_pos(t_ftl_root *l, FTL_NODE *pos, FTL_NODEC *node)
{
	t_ftl_node	*temp;

	temp = ft_memdup(node, l->node_size);
	if (temp == NULL)
		return (ENOMEM);
	temp->next = pos->next;
	temp->prev = pos;
	pos->next->prev = temp;
	pos->next = temp;
	l->size++;
	return (0);
}
