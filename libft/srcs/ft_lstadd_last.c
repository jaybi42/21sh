/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_last.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 14:00:23 by jguthert          #+#    #+#             */
/*   Updated: 2016/04/15 20:35:13 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_lstadd_last(t_list **alst, t_list *new)
{
	t_list	*cur;

	if (*alst == NULL)
	{
		new->next = NULL;
		(*alst) = new;
	}
	else
	{
		cur = *alst;
		while (cur->next != NULL)
			cur = cur->next;
		new->next = NULL;
		cur->next = new;
	}
}
