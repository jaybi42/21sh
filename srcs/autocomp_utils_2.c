/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 20:28:06 by mseinic           #+#    #+#             */
/*   Updated: 2016/10/15 16:40:15 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

static void			fill_in(char **tab, char **fill, int i)
{
	int				j;

	j = 0;
	while (fill[j] != NULL)
	{
		tab[i + j] = ft_strdup(fill[j]);
		j++;
	}
}

char            **append_found(char **tab, char *str, char *path)
{
	char        **ret;
	char        **tmp;
	int         i;
	int         j;

	ret = NULL;
	i = 0;
	j = 0;
	tmp = ret_tab(str, path);
	if (tab != NULL)
			i = tab_size(tab);
	if (tmp != NULL)
			j = tab_size(tmp);
	ret = (char **)malloc(sizeof(char *) * (i + j + 1));
	if (tab != NULL)
		fill_in(ret, tab, 0);
	if (tmp != NULL)
		fill_in(ret, tmp, i);
	del_tab(tmp);
	ret[i + j] = NULL;
	return (ret);
}
