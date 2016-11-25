/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 17:24:52 by mseinic           #+#    #+#             */
/*   Updated: 2016/11/10 02:38:30 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

void		ft_swap_print(t_line *l)
{
	int tmp;

	tmp = l->final_count;
	l->final_count = l->count;
	l->count = tmp;
	print(l);
}

int			cmp_buf(int *value, char *buf)
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

void		del_tab(char **t)
{
	int i;

	i = 0;
	if (t != NULL)
	{
		while (t[i] != NULL)
		{
			free(t[i]);
			i++;
		}
		free(t);
	}
}

int			tab_size(char **t)
{
	int i;

	i = 0;
	while (t[i] != NULL)
		i++;
	return (i);
}

char		**tab_dup(char **t)
{
	char		**ret;
	int			i;

	i = 0;
	while (t[i] != NULL)
		i++;
	ret = (char **)ft_memalloc(sizeof(char *) * (i + 1));
	i = 0;
	while (t[i] != NULL)
	{
		ret[i] = ft_strdup(t[i]);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
