/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_is_special.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/23 13:15:58 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/23 13:16:12 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

static t_actions const  g_actions[18] = {
	{ft_bp, {127, 0, 0, 0, 0, 0}, "BACKSPACE"},
	{ft_fw, {27, 91, 49, 59, 53, 67}, "CTRL R"},
	{NULL, {10, 0, 0, 0, 0, 0}, "ENTER"},
};

void			ft_fw(t_line *l)
{
	l->hist_pos -= 1;
}

void			ft_bp(t_line *l)
{
	(void)l;
}

static int		cmp_buf(int *value, char *buf)
{
    int         i;

    i = 0;
    while (i < 6)
    {
        if (buf[i] != value[i])
            return (1);
        i++;
    }
    return (0);
}

int				key_is_special(t_line *l)
{
    int         i;

    i = 0;
    while (g_actions[i].action != NULL)
    {
        if (cmp_buf((int *)g_actions[i].value, l->buffer) == 0)
        {
            g_actions[i].action(l);
            return (1);
        }
        i++;
    }
    return (0);
}
