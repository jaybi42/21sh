/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ctrl_r.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/23 13:14:43 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/23 14:46:10 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

static void	modify_str(t_line *l)
{
	//prendre les fonctions, ft_print_key sans le print
}

static bool	get_ans(t_ftl_root *h, int h_pos, char **ans, t_line *l)
{
	t_ftl_node	*node;

	node = (t_ftl_node *)h->prev;
	while (h_pos++ != h->size)
		node = node->prev;
	while (node != (t_ftl_node *)h)
	{
		if (ft_strstr(((t_hist *)node)->str, l->str) != NULL)
		{
			*ans = ((t_hist *)node)->str
			return (1);
		}
		l->hist_pos--; //need de le sauvegarder !
		node = node->prev;
	}
	return (0);
}

int			ctrl_r(t_line *old_l)
{
	bool	isok;
	char	*ans;
	char	*old_str;
	t_line	l;

	if (l->hist == NULL || l->hist.next == (t_ftl_node *)l->hist)
		return(0) ;
	l = ft_memdup(old_l);
	old_str = l->str;
	ans = NULL;
	while (1)
	{
		init
		print_ctrl_r(ans, isok, &l);
		if (read(0, l->buffer, 6) == -1)
			return (1);
		if (key_is_special(l) == 0)
			(l);
		isok = get_ans(l->hist, l->hist_pos, &ans, &l);
	}
	return (0);
}
