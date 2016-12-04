/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 17:10:59 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/04 17:40:18 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* A mettre dans un .h */
typedef struct		s_hash
{
	unsigned int	hash;
	char			*bin;
	char			*path;
	struct s_hash	*next;
}					t_hash;

void			init_bin_list(t_hash **addr, char *path);
unsigned int	hash_algo(char *key, int nb);
void			hash_del(t_hash ***hash, unsigned int hash_size);
char			*get_hash_path(t_hash **hash, char *bin, unsigned int g_hash_size);
int				collision = 0;
unsigned int	g_hash_size = 0;
/* A mettre dans un .h */

static unsigned int	list_size(t_hash *lst)
{
	unsigned int ret;

	ret = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		++ret;
	}
	return (ret);
}

static t_hash		*elem_list_dup(t_hash *master)
{
	t_hash	*new_node;

	if (!(new_node = (t_hash *)ft_memalloc(sizeof(t_hash))))
		return (NULL);
	new_node->bin =  ((master->bin != NULL) ? ft_strdup(master->bin) : NULL);
	new_node->path = ((master->path != NULL) ? ft_strdup(master->path) : NULL);
	new_node->next = NULL;
	return (new_node);
}

static void			insert_hash(t_hash ***hash, t_hash *lst, unsigned int nb)
{
	unsigned int	id;
	t_hash			*new_node;

	id = hash_algo(lst->bin, nb);
	if (!(new_node = elem_list_dup(lst)))
		return ;
	if ((*hash)[id] != NULL)
	{
		++collision;
		new_node->next = (*hash)[id]->next;
		(*hash)[id]->next = new_node;
	}
	else
		(*hash)[id] = new_node;
}

static void			lst_del(t_hash **addr)
{
	t_hash	*tmp;

	while (*addr)
	{
		if (((*addr)->bin) != NULL)
			ft_strdel(&((*addr)->bin));
		if (((*addr)->path) != NULL)
			ft_strdel(&((*addr)->path));
		tmp = *addr;
		*addr = (*addr)->next;
		if (tmp != NULL)
			ft_memdel((void **)&tmp);
	}
}

static t_hash		**hash_table(char *path)
{
	t_hash			*lst;
	t_hash			**hash;
	t_hash			*tmp;

	lst = NULL;
	init_bin_list(&lst, path);
	g_hash_size = list_size(lst);
	if (!(hash = (t_hash **)ft_memalloc(sizeof(t_hash *) * g_hash_size)))
		return (NULL);
	tmp = lst;
	while (tmp != NULL)
	{
		insert_hash(&hash, tmp, g_hash_size);
		tmp = tmp->next;
	}
	lst_del(&lst);
	return (hash);
}

int	main(void)
{
	t_hash	**hash;
	char	*p;
	char	*b = "ls";

	p = "/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/usr/local/munki:/Users/ibouchla/bin";
	hash = hash_table(p);
	printf("[%d] Path : %s\n", hash_algo(b, g_hash_size), get_hash_path(hash, b, g_hash_size));
	hash_del(&hash, g_hash_size);
	printf("Nombre de collisions : %d\n", collision);
/*	for (int id = 0; id < g_hash_size; id++)
	{
		if (hash[id] != NULL)
			for (int x = 0; (hash[id]); x++)
			{
				for (int z = 0; z < x; z++)
					ft_putstr("\t_____");
				printf("[%d] [bin] -> %s [path] -> : %s\n", id, hash[id]->bin, hash[id]->path);
				hash[id] = hash[id]->next;
			}
	}
*/
}
