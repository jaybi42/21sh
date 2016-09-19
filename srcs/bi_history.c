/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/30 17:13:12 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/19 18:17:00 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
** pop ligne
** print debut
** print fin
** history -c = del hist
** !n = print ligne n
** !-n = print ligne ($HISTSIZE - n)
*/

static void		del_history(t_ftl_node *temp)
{
	ft_strdel(&(((t_hist *)temp)->str));
}

static int		print_history(t_ftl_root *root, int size)
{
	t_ftl_node  *node;
	int			i;

	if ((t_ftl_root *)root->next == root)
		return (0);
	i = root->size - size;
	if (i < 0)
		i = 0;
	node = (t_ftl_node *)root->next;
	size = root->size - i;
	while (i-- > 0)
		node = node->next;
	while (node != (t_ftl_node *)root && size--)
	{
		ft_putstr("  ");
		ft_putnbr_fd(root->size - size, 2);
		ft_putstr(" ");
		ft_putendl_fd(((t_hist *)node)->str, 2);
		node = node->next;
		i++;
	}
	return (0);
}

static int			pop_line(t_ftl_root *hist, int pos)
{
	t_ftl_node	*node;

	if (ABS(pos) > (int)hist->size || hist->next == NULL)
		return (0);
	if (pos < 0)
		pos = hist->size + pos;
	node = (t_ftl_node *)hist->next;
	while (--pos > 0)
		node = node->next;
	ftl_erase_pos(hist, node, del_history);
	return (0);
}

int				bi_history(t_av av, t_ftl_root *hist)
{
	if (hist == NULL || hist->next == (t_ftl_node*)hist)
		return (0);
	else if (*av.arg == NULL)
		return (print_history(hist, 15));
	else if (!ft_strcmp(*av.arg, "-d"))
		return (pop_line(hist, ft_atoi(av.arg[1])));
	else if (!ft_strcmp(*av.arg, "-c"))
		ftl_release(hist, del_history);
	else if (*av.arg && ft_strncmp(*av.arg, "-", 1) == 0)
	{
		ft_putendl_fd("history: usage: history [-c] [-d offset] [n]", 2);
		return (1);
	}
	else if (!ft_strisnum(*av.arg))
		return (print_error(av, 11));
	else if (av.argc > 2)
		return (print_error(av, 0));
	return (print_history(hist, ft_atoi(*av.arg)));
}
