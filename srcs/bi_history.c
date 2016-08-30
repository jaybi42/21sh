/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/30 17:13:12 by jguthert          #+#    #+#             */
/*   Updated: 2016/08/30 18:39:37 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
** pop ligne
** print ligne
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

static int		print_history(t_ftl_root *root)
{
	t_ftl_node  *node;
	int			i;

	if ((t_ftl_node *)root->next == NULL)
		return ;
	i = 1;
	node = (t_ftl_node *)root->next;
	while (node != (t_ftl_node *)root)
	{
		ft_putstr("  ");
		ft_putnbr_fd(i, 2);
		ft_putstr(" ");
		ft_putendl_fd(((t_hist *)node)->str, 2);
		node = node->next;
		i++;
	}
	return (0);
}

static void			pop_line(t_ftl_root *hist, int pos)
{
	t_ftl_node	*node;

	if (ABS(pos) > hist->size)
		return (0);
	if (pos < 0)
		pos = hist->size + pos;
	while (pos-- > 0)
		node = node->next;
	ftl_erase_pos(hist, node, del_history);
}

static void			print_line(t_ftl_root *hist, int pos)
{
	t_ftl_node	*node;

	if (ABS(pos) > hist->size)
		return (0);
	if (pos < 0)
		pos = hist->size + pos;
	while (pos-- > 0)
		node = node->next;
	ft_putendl_fd(((t_hist *)node)->str, 2);
}

static int			clean_hist(t_ftl_root *hist)
{
}

int				bi_history(t_av av, t_ftl_root *hist)
{
	if (hist == NULL || hist->next == hist)
		return (0);
	if (*av.arg == NULL)
		return (print_history(hist));
	if (av.argc == 2 && ft_strcmp(*av.arg, "-d") == 0)
		return (pop_line(hist, ));
	if (av.argc == 1 && ft_strcmp(*av.arg, "-c") == 0)
	{
		ftl_release(&root1, del_history);
		return (0);
	}
//	print_line
//	print debut
//	print fin
	return (0);
}
