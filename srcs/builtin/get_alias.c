/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_alias.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/22 20:53:56 by ibouchla          #+#    #+#             */
/*   Updated: 2016/11/28 20:41:11 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <shell.h>

int		already_searched(t_list *key_list, char *new_key)
{
	if (!(new_key))
		return (1);
	while (key_list != NULL)
	{
		if ((ft_strcmp((char *)key_list->content, new_key)) == 0)
			return (1);
		key_list = key_list->next;
	}
	return (0);
}

void	*del_stuff(char **tmp, t_list **key_list)
{
	if (tmp)
		ft_strdel(tmp);
	if (key_list)
		ft_lstdel(key_list, &free_key);
	return (NULL);
}

char	*get_alias(char *key)
{
	char	*tmp;
	char	*ret;
	int		id;
	t_list	*key_list;

	if (!(key))
		return (NULL);
	tmp = ft_strdup(key);
	key_list = (t_list *)ft_memalloc(sizeof(t_list));
	while ((id = array_key_exists(g_alias, tmp)))
	{
		ft_lstadd(&key_list, ft_lstnew((void *)tmp, (ft_strlen(tmp) + 1)));
		ft_strdel(&tmp);
		if (!(tmp = get_alias_elem_by_id(g_alias, id)))
			return (del_stuff(NULL, &key_list));
		if ((already_searched(key_list, tmp)))
			return (del_stuff(&tmp, &key_list));
	}
	if ((array_key_exists(g_alias, key)))
	{
		ret = x_strdup(tmp);
		del_stuff(&tmp, &key_list);
		return (ret);
	}
	return (del_stuff(&tmp, &key_list));
}
