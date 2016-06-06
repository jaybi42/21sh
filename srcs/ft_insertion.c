/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insertion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 12:13:07 by malaine           #+#    #+#             */
/*   Updated: 2016/06/06 19:10:45 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include <stdlib.h>

char	*ft_insertion(int cursor, char *str, int size, char c)
{
	char	*strcpy;
	int		count;
	int		count2;

	count = 0;
	count2 = 0;
	strcpy = malloc(sizeof(char) * (ft_strlen(str) + 2));
	while (count < size + 1)
	{
		if (count == cursor)
			strcpy[count] = c;
		else
		{
			strcpy[count] = str[count2];
			count2++;
		}
		count++;
	}
	strcpy[count] = '\0';
	return (strcpy);
}

char	*ft_delete_char(int cursor, char *str, int size)
{
	char	*strcpy;
	int		count;
	int		count2;

	count = 0;
	count2 = 0;
	strcpy = malloc(sizeof(char) * (size));
	while (count < size)
	{
		if (count == cursor)
		{
			count2++;
			cursor = -1;
		}
		else
		{
			strcpy[count] = str[count2];
			count++;
			count2++;
		}
	}
	strcpy[count] = '\0';
	return (strcpy);
}
