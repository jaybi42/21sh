/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newfile.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 18:55:33 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/10 19:44:38 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include "shell.h"

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
		new_str = x_strdup("");
	else
		new_str = x_strndup(l->search, len - 1);
	if (new_str == NULL)
		return ;
	l->search = new_str;
	l->bp = 1;
}

static int		cmp_buf_bis(int *value, char *buf)
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
		if (cmp_buf_bis((int *)g_actions[i].value, l->buffer) == 0)
		{
			g_actions[i].action(l);
			return (1);
		}
		i++;
	}
	return (0);
}
