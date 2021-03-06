/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 17:10:59 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/05 17:46:32 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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
	new_node->bin = ((master->bin != NULL) ? ft_strdup(master->bin) : NULL);
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

t_hash				**hash_table(char *path)
{
	t_hash			*lst;
	t_hash			**hash;
	t_hash			*tmp;

	if (!(path))
		return (NULL);
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
