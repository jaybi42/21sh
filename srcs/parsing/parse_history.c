/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 16:58:08 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/04 20:38:55 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
** This file parse the char *str, in struct l
** if a "!foo" is found we replace it
**
** transform, will replace a word in "str" at pos "i" by "new_entry"
*/

static int		concat_str(char *t, char *new_i, char *t2, t_line *l)
{
	char	*t3;

	if (t != NULL)
	{
		t3 = ft_strjoin(t, new_i);
		if (t3 == NULL)
			return (1);
		ft_strdel(&new_i);
		ft_strdel(&t);
		new_i = t3;
	}
	if (t2 != NULL)
	{
		t3 = ft_strjoin(new_i, t2);
		if (t3 == NULL)
			return (1);
		ft_strdel(&new_i);
		ft_strdel(&t2);
		new_i = t3;
	}
	if (l->str != NULL)
		ft_strdel(&(l->str));
	l->str = new_i;
	return (0);
}

static int		transform_str(t_line *l, char *new_entry, int i)
{
	char	*temp;
	char	*temp2;

	temp = NULL;
	temp2 = NULL;
	if (new_entry == NULL)
		return (0);
	if (i != 0)
		temp = ft_strndup(l->str, i);
	while (l->str[i] != '\0' && ft_isspace(l->str[i]) == 0)
		i++;
	if (l->str + i != '\0')
		temp2 = ft_strdup(l->str + i);
	return (concat_str(temp, new_entry, temp2, l));
}

static int		replace_history(t_line *l, int i, t_ftl_root *hist)
{
	char	*new_entry;
	char	*tofind;

	tofind = ft_first_word(l->str + i);
	if (tofind == NULL)
		return (1);
	new_entry = get_event(tofind, hist);
	ft_strdel(&tofind);
	if (new_entry == NULL)
		return (1);
	transform_str(l, new_entry, i);
	return (0);
}

/*
** We check that "!" is not alone and not escaped
*/

int				parse_history(t_line *l, t_ftl_root *hist)
{
	int		i;
	bool	found;

	i = -1;
	found = 0;
	while (l->str[++i] != '\0')
	{
		if (l->str[i] != '!')
			continue ;
		else if ((i == 0 || (l->str[i - 1] != '\\' && l->str[i - 1] != '!'))
				&& l->str[i + 1] != '\0' && ft_isspace(l->str[i + 1]) == 0)
		{
			if (replace_history(l, i, hist) == 1)
				return (1);
			found = 1;
			i = -1;
		}
	}
	if (found == 1)
		ft_putendl(l->str);
	return (0);
}
