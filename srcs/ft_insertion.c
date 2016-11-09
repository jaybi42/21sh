/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insertion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 12:13:07 by malaine           #+#    #+#             */
/*   Updated: 2016/11/09 20:32:38 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include <stdlib.h>

int			ft_insertion(t_line *l)
{
	char	*str_cpy;
	int		count;
	int		count2;

	count = 0;
	count2 = 0;
	str_cpy = (char *)malloc(sizeof(char) * (ft_strlen(l->str) + 2));
	if (str_cpy == NULL)
		return (1);
	while (count < l->size + 1)
	{
		if (count == l->count)
			str_cpy[count] = l->buffer[0];
		else
		{
			str_cpy[count] = l->str[count2];
			count2++;
		}
		count++;
	}
	str_cpy[count] = '\0';
	ft_strdel(&l->str);
	l->str = str_cpy;
	return (0);
}

int			ft_delete_char(t_line *l)
{
	char	*str_cpy;
	int		count;
	int		count2;

	count = 0;
	count2 = 0;
	str_cpy = (char *)malloc(sizeof(char) * (l->size));
	if (str_cpy == NULL)
		return (1);
	while (count < l->size)
	{
		str_cpy[count] = l->str[count2];
		if (count == l->count - 1)
			count2++;
		count++;
		count2++;
	}
	str_cpy[count] = '\0';
	ft_strdel(&l->str);
	l->str = str_cpy;
	return (0);
}
