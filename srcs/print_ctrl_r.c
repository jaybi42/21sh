/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ctrl_r.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/23 13:40:57 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/23 15:31:15 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

static char	*construct_search(char *search)
{
	char	*tmp;

	if (search == NULL)
		return (ft_strdup("_"));
	tmp = ft_strjoin(search, "_");
	return (tmp);
}

static char	*join_all(t_line *l, char *search, bool isok)
{
	char	*tmp1;
	char	*tmp2;

	if (l->ans == NULL)
		l->ans = ft_strdup("");
	if (l->ans == NULL)
		return (NULL);
	if (isok)
		tmp1 = ft_strjoin(l->ans, "\n  research : ");
	else
		tmp1 = ft_strjoin(l->ans, "\n  failing searching : ");
	if (tmp1 == NULL)
		return (NULL);
	tmp2 = ft_strjoin(tmp1, search);
	ft_strdel(&tmp1);
	return (tmp2);
}

int			constructor_search(bool isok, t_line *l)
{
	char	*full_str;
	char	*new_search;

	new_search = construct_search(l->search);
	if (new_search == NULL)
		return (1);
	full_str = join_all(l, new_search, isok);
	ft_strdel(&new_search);
	if (full_str == NULL)
		return (1);
	l->str = full_str;
	return (0);
}
