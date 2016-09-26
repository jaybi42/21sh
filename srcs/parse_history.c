/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 16:58:08 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/25 19:10:46 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
** This file parse the char *str, in struct l
** if a "!foo" is found we replace it
*/

static int		transform_str(t_line *l, char *new_entry, int i)
{
	char	*temp;
	int		len;

	if (i != 0)
	{
		temp = ft_strndup(l->str, i - 1);
		if (temp == NULL)
			return (1);
		new_entry = ft_strjoin(temp, new_entry);
		ft_strdel(&temp);
		if (new_entry == NULL)
			return (1);
	}
	len = 0;
	while (l->str[len] != '\0' && ft_isblank(l->str[len]) == 0)
		len++;
	temp = ft_strjoin(new_entry, l->str + len);
	if (temp == NULL)
		return (1);
	ft_strdel(&(l->str));
	l->str = temp;
	return (0);
}

void			parse_history(t_line *l, t_ftl_root *hist)
{
	char	*new_entry;
	int		i;

	i = 0;
	while (l->str[i] != '\0')
	{
		if (l->str[i] == '!' && (i == 0 || l->str[i - 1] != '\\'))
		{
			new_entry = get_event(l->str, hist);
			if (new_entry != NULL)
			{
				transform_str(l, new_entry, i);
				ft_strdel(&new_entry);
			}
		}
		i++;
	}
}
