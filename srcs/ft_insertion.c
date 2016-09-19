/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insertion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 12:13:07 by malaine           #+#    #+#             */
/*   Updated: 2016/09/16 15:48:49 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include <stdlib.h>

char	*ft_insertion(int cursor, char *str, int size, char c)
{
	char	*str_cpy;
	int		count;
	int		count2;

	count = 0;
	count2 = 0;
	str_cpy = (char *)malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (str_cpy == NULL)
		return (NULL);
	while (count < size + 1)
	{
		if (count == cursor)
			str_cpy[count] = c;
		else
		{
			str_cpy[count] = str[count2];
			count2++;
		}
		count++;
	}
	str_cpy[count] = '\0';
	return (str_cpy);
}

char	*ft_delete_char(int cursor, char *str, int size)
{
	char	*str_cpy;
	int		count;
	int		count2;

	count = 0;
	count2 = 0;
	str_cpy = (char *)malloc(sizeof(char) * (size));
	if (str_cpy == NULL)
		return (NULL);
	while (count < size)
	{
		if (count == cursor)
		{
			count2++;
			cursor = -1;
		}
		else
		{
			str_cpy[count] = str[count2];
			count++;
			count2++;
		}
	}
	str_cpy[count] = '\0';
	return (str_cpy);
}
