/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newfile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 18:55:33 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/16 18:05:31 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

static t_actions const g_actions[18] = {
	{ft_bp, {127, 0, 0, 0, 0, 0}, "BACKSPACE"},
	{ft_fw, {18, 0, 0, 0, 0, 0}, "CTRL R"},
	{NULL, {10, 0, 0, 0, 0, 0}, "ENTER"},
};

void			ft_fw(t_line *l)
{
	if (ft_strcmp(l->ans, "") != 0 && l->hist_pos > 0)
		l->hist_pos -= 1;
}

void			ft_bp(t_line *l)
{
	char	*new_str;
	int		len;

	if (l->search == NULL)
		return ;
	len = ft_strlen(l->search);
	if (len <= 1)
		new_str = ft_strdup("");
	else
		new_str = ft_strndup(l->search, len - 1);
	if (new_str == NULL)
		return ;
	ft_strdel(&l->search);
	l->search = new_str;
	l->bp = 1;
}

static int		cmp_buf(int *value, char *buf)
{
	int		i;

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
	int		i;

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