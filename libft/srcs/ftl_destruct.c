/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftl_destruct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 18:00:40 by jguthert          #+#    #+#             */
/*   Updated: 2016/08/24 15:11:02 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void			ftl_release(t_ftl_root *l, void (*delalloc)())
{
	while (l->size > 0)
		ftl_pop_front(l, delalloc);
	*l = ftl_uninit();
}

void			ftl_pop_back(t_ftl_root *l, void (*delalloc)())
{
	t_ftl_node	*temp;

	temp = (t_ftl_node *)l->prev;
	temp->prev->next = temp->next;
	l->prev = temp->prev;
    if (delalloc != NULL)
        delalloc(temp);
	free(temp);
	l->size--;
}

void			ftl_pop_front(t_ftl_root *l, void (*delalloc)())
{
	t_ftl_node	*temp;

	temp = (t_ftl_node *)l->next;
	temp->next->prev = temp->prev;
	l->next = temp->next;
    if (delalloc != NULL)
        delalloc(temp);
	free(temp);
	l->size--;
}

void			ftl_erase_pos(t_ftl_root *l, t_ftl_node *pos, void (*delalloc)())
{
	pos->next->prev = pos->prev;
	pos->prev->next = pos->next;
	if (delalloc != NULL)
		delalloc(pos);
	free(pos);
	l->size--;
}
