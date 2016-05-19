/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/03 21:59:27 by jguthert          #+#    #+#             */
/*   Updated: 2016/01/03 13:28:30 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	i = ft_strlen(s1);
	j = ft_strlen(s2);
	if (!(str = (char*)malloc(sizeof(char) * (i + j + 1))))
		return (NULL);
	str[i + j] = '\0';
	while (j--)
		str[j + i] = s2[j];
	while (i--)
		str[i] = s1[i];
	return (str);
}
