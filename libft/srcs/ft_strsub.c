/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilberm <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 19:51:01 by tsilberm          #+#    #+#             */
/*   Updated: 2016/11/07 20:56:32 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*str;
	int		i;

	str = NULL;
	if (s && (int)len >= 0)
	{
		i = 0;
		if (!(str = (char *)malloc(sizeof(char) * (len + 1))))
			return (NULL);
		str[len] = '\0';
		while (len--)
			str[i++] = s[start++];
	}
	return (str);
}
