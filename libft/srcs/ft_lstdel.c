/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 15:17:42 by jguthert          #+#    #+#             */
/*   Updated: 2016/04/16 19:05:56 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void		ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list	*l;
	t_list	*p;

	if (alst != NULL)
	{
		l = *alst;
		while (l != NULL)
		{
			p = l;
			l = l->next;
			ft_lstdelone(&p, del);
		}
		*alst = NULL;
	}
}
