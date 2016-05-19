/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 15:15:25 by jguthert          #+#    #+#             */
/*   Updated: 2016/04/17 16:56:37 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*newlist;
	void	*new_content;

	if (!(newlist = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	else
	{
		if (content == NULL)
		{
			newlist->content = NULL;
			newlist->content_size = 0;
		}
		else
		{
			new_content = ft_memalloc(content_size);
			if (new_content == NULL)
				ft_putendl_fd("C'EST NULL", 2);
			ft_memcpy(new_content, content, content_size);
			newlist->content = new_content;
			newlist->content_size = content_size;
		}
		newlist->next = NULL;
	}
	return (newlist);
}
