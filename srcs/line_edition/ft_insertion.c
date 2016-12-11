/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insertion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 12:13:07 by malaine           #+#    #+#             */
/*   Updated: 2016/12/10 19:40:26 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include "shell.h"
#include <stdlib.h>

int			ft_insertion(t_line *l)
{
	char	*str_cpy;
	int		count;
	int		count2;

	count = 0;
	count2 = 0;
	str_cpy = (char *)xmalloc(sizeof(char) * (ft_strlen(l->str) + 2));
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
	l->str = NULL;
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
	str_cpy = (char *)xmalloc(sizeof(char) * (l->size + 1));
	if (str_cpy == NULL)
		return (1);
	while (count < l->size)
	{
		if (l->count == 0 && count == l->count)
			count2++;
		str_cpy[count] = l->str[count2];
		if (l->count != 0 && count == l->count - 1)
			count2++;
		count++;
		count2++;
	}
	str_cpy[count] = '\0';
	l->str = NULL;
	l->str = str_cpy;
	return (0);
}
