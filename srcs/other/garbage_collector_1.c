/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:36:36 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/06 19:36:38 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void			*xmalloc(size_t size)
{
	void *al;

	if ((al = malloc(size)) == NULL)
		return (NULL);
	storage(INSERT, al);
	return (al);
}

void			xmemdel(void **p)
{
	storage(DELETE, (*p));
	(*p) = NULL;
}

/*
**	delete all the mem created in a safe way!
*/

void			xmasterfree(void)
{
	storage(DELETEALL, NULL);
}

/*
**	print the mem allocated
*/

void			xprintmem(void)
{
	storage(PRINT, NULL);
}
