/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 17:13:02 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/08 18:16:09 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <math.h>

unsigned int	hash_algo(char *key, int nb)
{
	int				i;
	unsigned int	hash;

	i = (-1);
	hash = 0;
	if (!(key) || nb <= 0)
		return (0);
	while (key[++i])
		hash += key[i] * (unsigned int)pow(5.0, i);
	return (hash %= nb);
}

char			*get_hash_path(t_hash ***hash_addr, char *bin)
{
	unsigned int	id;
	t_hash			*hash;

	if (!(*hash_addr) || !(bin))
		return (NULL);
	id = hash_algo(bin, g_hash_size);
	hash = (*hash_addr)[id];
	while (hash)
	{
		if (hash->bin != NULL)
			if ((ft_strcmp(hash->bin, bin)) == 0)
				return (hash->path);
		hash = hash->next;
	}
	return (NULL);
}

void			hash_del(t_hash ***hash)
{
	unsigned int	i;
	t_hash			*tmp;

	i = (-1);
	tmp = NULL;
	while (++i < g_hash_size)
		if ((*(*hash + i)) != NULL)
			while ((*(*hash + i)) != NULL)
			{
				if ((*(*hash + i))->bin != NULL)
					ft_strdel(&((*(*hash + i))->bin));
				if ((*(*hash + i))->path != NULL)
					ft_strdel(&((*(*hash + i))->path));
				tmp = (*(*hash + i));
				(*(*hash + i)) = (*(*hash + i))->next;
				if (tmp != NULL)
					ft_memdel((void **)&tmp);
			}
	ft_memdel((void **)hash);
	g_hash_size = 0;
}
