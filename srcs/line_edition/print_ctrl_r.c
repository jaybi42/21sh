/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ctrl_r.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/10 03:00:32 by malaine           #+#    #+#             */
/*   Updated: 2016/12/10 19:43:06 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include "shell.h"

static int		inst_ans(t_line *l)
{
	l->ans = x_strdup(l->oldstr);
	if (l->ans == NULL)
		return (1);
	return (0);
}

static char		*join_all(t_line *l, char *search, bool isok)
{
	char	*tmp2;
	char	*tmp1;

	if (ft_strcmp(search, "_") == 0)
	{
		if (inst_ans(l) == 1)
			return (NULL);
		tmp1 = ft_strjoin(l->ans, "  research :  ");
	}
	else if (isok)
		tmp1 = ft_strjoin(l->ans, "  research :  ");
	else
		tmp1 = ft_strjoin(l->ans, "  failing search : ");
	if (tmp1 == NULL)
		return (NULL);
	tmp2 = x_strjoin(tmp1, search);
	ft_strdel(&tmp1);
	return (tmp2);
}

int				constructor_search(bool isok, t_line *l)
{
	char	*full_str;
	char	*new_search;

	new_search = ft_strjoin(l->search, "_");
	if (new_search == NULL)
		return (1);
	full_str = join_all(l, new_search, isok);
	ft_strdel(&new_search);
	if (full_str == NULL)
		return (1);
	l->str = full_str;
	l->final_count = ft_strlen(l->ans);
	l->size = ft_strlen(l->ans);
	return (0);
}
