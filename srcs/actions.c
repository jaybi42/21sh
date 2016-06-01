/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/13 13:49:16 by jguthert          #+#    #+#             */
/*   Updated: 2016/06/01 18:02:53 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <unistd.h>

static t_actions const	g_actions[16] = {
	{ft_right, {27, 91, 65, 0, 0, 0}, "UP"},
	{ft_left, {27, 91, 66, 0, 0, 0}, "DOWN"},
	{ft_right, {27, 91, 67, 0, 0, 0}, "RIGHT"},
	{ft_left, {27, 91, 68, 0, 0, 0}, "LEFT"},
	{del_action, {27, 91, 51, 126, 0, 0}, "DELETE"},
	{ft_backspace, {127, 0, 0, 0, 0, 0}, "BACKSPACE"},
	{ft_ctrl_r, {0, 0, 0, 0, 0, 67}, "CTRL R"},
	{ft_ctrl_l, {0, 0, 0, 0, 0, 68}, "CTRL L"},
	{ft_home, {0, 91, 72, 0, 0, 0}, "HOME"},
	{ft_end, {0, 91, 70, 0, 0, 0}, "END"},
	{ft_cut, {11, 0, 0, 0, 0, 0}, "CUT"},
	{ft_paste, {16, 0, 0, 0, 0, 0}, "PASTE"},
	{ft_ctrl_up, {0, 0, 0, 59, 53, 65}, "CTRL UP"},
	{ft_ctrl_down, {0, 0, 0, 59, 53, 66}, "CTRL DOWN"},
	{NULL, {10, 0, 0, 0, 0, 0}, "ENTER"},
	{ft_print_key, {0, 0, 0, 0, 0, 0}, "Normal Key"},
};

static int		cmp_buf(int *value, char *buf)
{
	int			i;

	i = 0;
	while (i < 6)
	{
		if (buf[i] != value[i])
			return (1);
		i++;
	}
	return (0);
}

int				actions(t_line *l)
{
	int			i;

	i = 0;
	while (g_action[i].action != NULL)
	{
		if (cmp_buf((int *)g_actions[i].value, l->buffer) == 0)
		{
			g_actions[i].action(l);
			return (0);
		}
		i++;
	}
	return (1);
}
