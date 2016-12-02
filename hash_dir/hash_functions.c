/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 17:13:02 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/02 17:13:29 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

/* A mettre dans un .h */
typedef struct		s_hash
{
	unsigned int	hash;
	char			*bin;
	char			*path;
	struct s_hash	*next;
}					t_hash;
/* A mettre dans un .h */

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

char			*get_hash_path(t_hash **hash, char *bin, unsigned int g_hash_size)
{
	unsigned int	id;

	if (!(hash) || !(bin))
		return (NULL);
	id = hash_algo(bin, g_hash_size);
	while (hash[id])
	{
		if ((hash[id])->bin != NULL)
			if ((ft_strcmp((hash[id])->bin, bin)) == 0)
				return ((hash[id])->path);
		hash[id] = (hash[id])->next; 
	}
	return (NULL);
}

void			hash_del(t_hash ***hash, unsigned int g_hash_size)
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
}
