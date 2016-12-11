/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_up_down.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 20:41:47 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/10 19:42:25 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char		*get_hist_pos(t_ftl_root *hist, int pos, int dir, t_line *l)
{
	t_ftl_node	*node;

	node = (t_ftl_node *)hist->prev;
	while (pos++ != (int)hist->size)
		node = node->prev;
	if (dir == 1)
		node = node->next;
	else if (dir == -1)
		node = node->prev;
	l->hist_pos += dir;
	return (((t_hist *)node)->str);
}

static char		*get_str_in_hist(t_line *l, int dir)
{
	if (l->hist_pos == -1)
	{
		if (dir == 1)
			return (NULL);
		l->hist_pos = l->hist->size;
		l->oldstr = l->str;
		l->str = NULL;
		return (get_hist_pos(l->hist, l->hist_pos, 0, l));
	}
	else if (l->hist_pos == (int)l->hist->size && dir == 1)
	{
		l->hist_pos = -1;
		return (l->oldstr);
	}
	else if (l->hist_pos == 1 && dir == -1)
		return (NULL);
	else
		return (get_hist_pos(l->hist, l->hist_pos, dir, l));
}

static void		dup_print(char *new_in, t_line *l)
{
	char	*tmp;

	if (new_in != NULL)
	{
		tmp = x_strdup(new_in);
		if (tmp == NULL)
			return ;
		l->str = tmp;
		l->final_count = ft_strlen(l->str);
		l->size = l->final_count;
		print(l);
	}
	else
		l->size = 0;
}

void			ft_up(t_line *l)
{
	char	*new_in;

	if (l->hist == NULL || l->hist->prev == (t_ftl_node *)l->hist)
		return ;
	new_in = get_str_in_hist(l, -1);
	if (new_in == NULL)
		return ;
	dup_print(new_in, l);
}

void			ft_down(t_line *l)
{
	char	*new_in;

	if (l->hist == NULL || l->hist->next == (t_ftl_node *)l->hist)
		return ;
	new_in = get_str_in_hist(l, 1);
	dup_print(new_in, l);
}
