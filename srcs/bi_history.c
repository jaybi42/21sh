/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/30 17:13:12 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/05 18:24:34 by jguthert         ###   ########.fr       */
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
		return (0);
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

static int			print_line(t_ftl_root *hist, int pos)
{
	t_ftl_node	*node;

	if (ABS(pos) > (int)hist->size || hist->next == NULL)
		return (0);
	if (pos < 0)
		pos = hist->size + pos;
	node = (t_ftl_node *)hist->next;
	while (--pos > 0)
		node = node->next;
	ft_putendl_fd(((t_hist *)node)->str, 2);
	return (0);
}

static int		parse_history(t_av av, t_ftl_root *hist)
{
	(void)av;
	(void)hist;
//	print_line
//	print debut
//	print fin
	return (0);
}

int				bi_history(t_av av, t_ftl_root *hist)
{
	if (hist == NULL || hist->next == (t_ftl_node*)hist)
		return (0);
	else if (*av.arg == NULL && ft_strcmp("history", av.cmd) == 0)
		return (print_history(hist));
	else if (av.argc == 2 && ft_strcmp(*av.arg, "-d") == 0)
		return (pop_line(hist, ft_atoi(av.arg[1])));
	else if (av.argc == 1 && ft_strcmp(*av.arg, "-c") == 0)
	{
		ftl_release(hist, del_history);
	return (0);
	}
//	else
//		parse_history();
	return (0);
}
