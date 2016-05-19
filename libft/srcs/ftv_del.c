/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftv_del.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/03 15:09:11 by jguthert          #+#    #+#             */
/*   Updated: 2016/01/03 17:58:54 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void		ftv_del_len(t_ftv *v, size_t s, size_t e, void (*del)())
{
	void			*cur;
	void *const		end = v->data + e * v->element_size;

	if (del != NULL)
	{
		cur = v->data + s * v->element_size;
		while (cur != end)
		{
			del(cur);
			cur += v->element_size;
		}
	}
	if (v->size > 0)
		free(v->data);
}

void		ftv_del(t_ftv *v, void (*del)())
{
	void			*cur;
	void *const		end = v->data + v->size * v->element_size;

	if (del != NULL)
	{
		cur = v->data;
		while (cur != end)
		{
			del(cur);
			cur += v->element_size;
		}
	}
	if (v->size > 0)
		free(v->data);
}

void		ftv_popback(t_ftv *v, void (*del)())
{
	if (del != NULL)
		del(v->data + (v->size - 1) * v->element_size);
	v->size--;
}
