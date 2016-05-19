/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/13 13:49:16 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/16 17:58:20 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <unistd.h>

static t_actions const	g_actions[7] = {
	{up_action, {27, 91, 65, 0}, "UP"},
	{down_action, {27, 91, 66, 0}, "DOWN"},
	{right_action, {27, 91, 67, 0}, "RIGHT"},
	{left_action, {27, 91, 68, 0}, "LEFT"},
	{del_action, {27, 91, 51, 126}, "DELETE"},
	{del_action, {127, 0, 0, 0}, "BACKSPACE"},
	{sel_action, {32, 0, 0, 0}, "SPACEBAR"},
};

static int		cmp_buf(int *value, char *buf)
{
	int			i;

	i = 0;
	while (i < 4)
	{
		if (buf[i] != value[i])
			return (1);
		i++;
	}
	return (0);
}

int				actions(char *buf, t_ftl_root *root, t_ftl_node **pos)
{
	int			i;

	i = 0;
	while (i < 7)
	{
		if (cmp_buf((int *)g_actions[i].value, buf) == 0)
		{
			g_actions[i].action(root, pos);
			return (0);
		}
		i++;
	}
	return (0);
}
