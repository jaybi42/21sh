/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_up_down.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/12 14:26:25 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/12 18:32:28 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static char		*get_hist_pos(t_ftl_root *hist, int pos, int dir, t_line *l)
{
	t_ftl_node	*node;

	node = (t_ftl_node *)hist->prev;
	while (pos++ != (int)hist->size)
		node = node->prev;
	if (dir == 1)
		node = node->next;
	else
		node = node->prev;
	if (node == (t_ftl_node *)hist)
		return (NULL);
	if (!((l->hist_pos == 0 && dir == -1)
		  || (l->hist_pos == (int)hist->size && dir == 1)))
		l->hist_pos += dir;
	return (((t_hist *)node)->str);
}


void			ft_up(t_line *l)
{
	char	*new_in;

	if (l->hist == NULL || l->hist->prev == NULL)
		return ;
	if ((int)l->hist->size == l->hist_pos && l->str != NULL)
		add_history(l->str, l->hist);
	new_in = get_hist_pos(l->hist, l->hist_pos, -1, l);
	if (new_in != NULL)
	{
		if (l->str != NULL)
			ft_strdel(&l->str);
		l->str = ft_strdup(new_in);
		l->count = ft_strlen(l->str);
		l->size = l->count;
		ft_print_line(l);
	}
}

void			ft_down(t_line *l)
{
	char	*new_in;

	if (l->hist == NULL || l->hist->next == (t_ftl_node *)l->hist)
		return ;
	new_in = get_hist_pos(l->hist, l->hist_pos, 1, l);
	if (new_in != NULL)
	{
		if (l->str != NULL)
			ft_strdel(&l->str);
		l->str = ft_strdup(new_in);
		l->count = ft_strlen(l->str);
		l->size = l->count;
		ft_print_line(l);
	}
}
