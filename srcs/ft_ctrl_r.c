/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ctrl_r.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/23 13:14:43 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/23 15:30:52 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static bool	get_ans(t_ftl_root *h, int h_pos, t_line *l)
{
	t_ftl_node	*node;

	node = (t_ftl_node *)h->prev;
	while (h_pos++ != (int)h->size)
		node = node->prev;
	while (node != (t_ftl_node *)h)
	{
		if (ft_strstr(((t_hist *)node)->str, l->str) != NULL)
		{
			l->ans = ((t_hist *)node)->str;
			return (1);
		}
		l->hist_pos--;
		node = node->prev;
	}
	return (0);
}

static void	ft_addchar(t_line *l)
{
	char	*tmp;

	if (l->search == NULL)
		tmp = ft_strdup(&l->buffer[0]);
	else
	{
		tmp = ft_strjoin(l->search, &l->buffer[0]);
		ft_strdel(&(l->search));
	}
    if (tmp == NULL)
        return ;
	l->search = tmp;
}

static void	clean_ctrl_r(t_line *l)
{
	if (l->str != NULL)
		ft_strdel(&l->str);
	if (l->search != NULL)
		ft_strdel(&l->search);
	if (l->ans != NULL)
	{
		l->str = l->ans;
		l->ans = NULL;
	}
	if (l->oldstr != NULL)
		ft_strdel(&l->oldstr);
}

void		ctrl_r(t_line *l)
{
	bool	isok;

	l->oldstr = l->str;
	l->str = NULL;
	isok = 1;
	while (1)
	{
		constructor_search(isok, l);
		if (read(0, l->buffer, 6) == -1)
		{
			clean_ctrl_r(l);
			return ;
		}
		if (key_is_special(l) == 0)
		{
			if (ft_isprint(l->buffer[0]) == 1)
				ft_addchar(l);
			else
			{
				clean_ctrl_r(l);
				return ;
			}
		}
		isok = get_ans(l->hist, l->hist_pos, l);
	}
}
