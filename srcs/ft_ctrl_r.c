/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newfile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 18:39:18 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/16 18:43:52 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static bool		get_ans(t_ftl_root *h, int h_pos, t_line *l)
{
	t_ftl_node	*node;
	int			i;

	i = 0;
	node = (t_ftl_node *)h->next;
	if (h_pos <= -1)
		node = (t_ftl_node *)h->prev;
	while (--h_pos > 0)
		node = node->next;
	while (node != (t_ftl_node *)h)
	{
		if (ft_strstr(((t_hist *)node)->str, l->search) != NULL)
		{
			ft_strdel(&l->ans);
			l->hist_pos -= i;
			l->bp = 0;
			l->ans = ft_strdup(((t_hist *)node)->str);
			return (1);
		}
		i++;
		if (l->bp == 1 && (l->bp = 0) == 0)
			return (get_ans(h, -1, l));
		node = node->prev;
	}
	return (0);
}

static void		ft_addchar(t_line *l)
{
	char		*tmp;

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
	l->hist_pos = -1;
}

static void		clean_ctrl_r(t_line *l)
{
	if (l->quit != -1)
	{
		l->hist_pos = -1;
		l->bp = 0;
		if (l->str != NULL)
			ft_strdel(&l->str);
		if (l->search != NULL)
			ft_strdel(&l->search);
		if (l->ans != NULL)
		{
			l->str = l->ans;
			l->size = ft_strlen(l->str);
			l->ans = NULL;
		}
		ft_strdel(&l->oldstr);
		print(l);
	}
	l->quit = 0;
}

static int		init_ctrl_r(t_line *l)
{
	l->search = ft_strdup("");
	l->ans = ft_strdup("");
	l->bp = 0;
	if (l->search == NULL || l->ans == NULL)
		return (1);
	if (l->str == NULL)
	{
		l->oldstr = ft_strdup("");
		if (l->oldstr == NULL)
			return (1);
	}
	else
	{
		l->oldstr = l->str;
		l->str = NULL;
	}
	l->hist_pos = -1;
	return (0);
}

void			ctrl_r(t_line *l)
{
	bool	isok;

	if ((isok = init_ctrl_r(l)) == 1)
		return ;
	while (1)
	{
		if (constructor_search(isok, l) == 0)
			print(l);
		ft_bzero(l->buffer, 6);
		if (read(0, l->buffer, 6) == -1 || l->quit == -1)
			return (clean_ctrl_r(l));
		if (key_is_special(l) == 0)
		{
			if (ft_isprint(l->buffer[0]) == 1)
			{
				if (isok == 1 || ft_strcmp(l->search, "") == 0)
					ft_addchar(l);
			}
			else
				return (clean_ctrl_r(l));
		}
		isok = get_ans(l->hist, l->hist_pos, l);
		l->bp = 0;
	}
}
