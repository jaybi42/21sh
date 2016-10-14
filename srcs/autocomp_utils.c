/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 17:24:52 by mseinic           #+#    #+#             */
/*   Updated: 2016/10/14 20:25:03 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

int	cmp_buf(int *value, char *buf)
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

void		del_tab(char **tab)
{
	int i;

	i = 0;
	if (tab != NULL)
	{
		while (tab[i] != NULL)
		{
			free(tab[i]);
			i++;
		}
		free(tab);
	}
}

int			tab_size(char **tab)
{
	int i;

	i = 0;
	while (tab[i] != NULL)
		i++;
	return (i);
}

char		**tab_dup(char **tab)
{
	char		**ret;
	int			i;

	i = 0;
	while (tab[i] != NULL)
		i++;
	ret = (char **)ft_memalloc(sizeof(char *) * (i + 1));
	i = 0;
	while (tab[i] != NULL)
	{
		ret[i] = ft_strdup(tab[i]);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
