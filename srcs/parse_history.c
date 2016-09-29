/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 16:58:08 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/27 18:52:30 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
** This file parse the char *str, in struct l
** if a "!foo" is found we replace it
**
** transform, will replace a word in "str" at pos "i" by "new_entry"
*/

static int		transform_str(t_line *l, char *new_entry, int i)
{
	char	*temp;
	char	*temp2;

	if (new_entry == NULL)
		return (0);
	if (i != 0)
	{
		temp = ft_strndup(l->str, i);
		if (temp == NULL)
			return (1);
		temp2 = ft_strjoin(temp, new_entry);
		ft_strdel(&temp);
		ft_strdel(&new_entry);
		if (temp2 == NULL)
			return (1);
	}
	while (l->str[i] != '\0' && ft_isspace(l->str[i]) == 0)
		i++;
	temp = ft_strjoin(temp2, l->str + i);
	if (temp == NULL)
		return (1);
	ft_strdel(&(l->str));
	l->str = temp;
	return (0);
}

/*
** We check that "!" is not alone and not escaped
*/

int				parse_history(t_line *l, t_ftl_root *hist)
{
	char	*new_entry;
	char	*tofind;
	int		i;

	i = -1;
	while (l->str[++i] != '\0')
	{
		if (l->str[i] != '!')
			continue ;
		else if ((i == 0 || (l->str[i - 1] != '\\' && l->str[i - 1] != '!'))
				 && l->str[i + 1] != '\0' && ft_isspace(l->str[i + 1]) == 0)
		{
			/*
				jb probleme a cette ligne (commenté):
			*/
			//tofind = ft_first_word(l->str + i);
			if (tofind == NULL)
				return (1);
			new_entry = get_event(tofind, hist);
			ft_strdel(&tofind);
			if (new_entry == NULL)
				return (1);
			transform_str(l, new_entry, i);
			ft_putendl(l->str);
		}
	}
	return (0);
}
