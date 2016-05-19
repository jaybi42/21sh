/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_select.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/13 17:50:04 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/18 17:51:22 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static int			error_size(void)
{
	do_termcap("cl");
	ft_putstr_fd("ft_select : TOO SMALL windows size.", 2);
	return (1);
}

static int			print_selected(char *name, bool cursor)
{
	if (cursor == 1)
	{
		ft_putstr_fd(H_RED, 2);
		do_termcap("us");
	}
	else
		ft_putstr_fd(H_BLUE, 2);
	do_termcap("so");
	ft_putendl_fd(name, 2);
	ft_putstr_fd(END, 2);
	do_termcap("me");
	ft_putstr_fd(END, 2);
	return (0);
}

static int			print_normal(char *name, bool cursor)
{
	if (cursor == 1)
	{
		ft_putstr_fd(H_RED, 2);
		do_termcap("us");
		ft_putendl_fd(name, 2);
		do_termcap("ue");
		ft_putstr_fd(END, 2);
	}
	else
		ft_putendl_fd(name, 2);
	return (0);
}

int					print_select(t_ftl_root *root)
{
	t_ftl_node		*node;

	do_termcap("cl");
	node = (t_ftl_node *)root->next;
	print_header();
	while (node != (t_ftl_node *)root)
	{
		if (padding_control(root, node) == 1)
			return (error_size());
		if (((t_elem *)node)->selected == 1)
			print_selected(((t_elem *)node)->name, ((t_elem *)node)->cursor);
		else
			print_normal(((t_elem *)node)->name, ((t_elem *)node)->cursor);
		node = node->next;
	}
	return (0);
}
