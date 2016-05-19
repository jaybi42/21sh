/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector_insert.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/03 13:52:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/01/03 18:00:31 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

int			ftv_reserve(t_ftv *v, size_t new_cap)
{
	void	*tmp;

	if (new_cap <= v->capacity)
		return (0);
	tmp = (void *)malloc(v->element_size * new_cap);
	if (tmp == NULL)
		return (1);
	if (v->size > 0)
		ft_memcpy(tmp, v->data, v->capacity * v->element_size);
	if (v->data != NULL)
		free(v->data);
	v->capacity = new_cap;
	v->data = tmp;
	return (0);
}

int			ftv_pushback(t_ftv *v, void const *new_data)
{
	if (v->capacity == v->size)
	{
		if (ftv_reserve(v, v->capacity == 0 ? 8 : v->capacity * 2))
			return (1);
	}
	ft_memcpy(v->data + v->size * v->element_size, new_data, v->element_size);
	v->size++;
	return (0);
}
