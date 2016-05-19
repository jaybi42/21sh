/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 15:15:00 by jguthert          #+#    #+#             */
/*   Updated: 2015/12/19 19:11:21 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*begin_list;
	t_list	*end_list;
	t_list	*tmp;

	if (lst)
	{
		tmp = f(lst);
		if (!(begin_list = ft_lstnew(tmp->content, tmp->content_size)))
			return (NULL);
		end_list = begin_list;
		while ((lst = lst->next))
		{
			tmp = f(lst);
			if (!(tmp = ft_lstnew(tmp->content, tmp->content_size)))
				return (NULL);
			end_list->next = tmp;
			end_list = tmp;
		}
		return (begin_list);
	}
	return (NULL);
}
