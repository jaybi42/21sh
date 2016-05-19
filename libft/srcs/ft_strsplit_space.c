/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit_space.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/30 21:37:53 by jguthert          #+#    #+#             */
/*   Updated: 2016/04/17 15:19:54 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int			count_words(char *s)
{
	int		words;

	while (*s && ft_isspace(*s) == 1)
		++s;
	words = (*s) ? 1 : 0;
	while (*s)
	{
		if (ft_isspace(*s) == 1 && s[1] && ft_isspace(s[1]) == 0)
			++words;
		++s;
	}
	return (words);
}

char				**ft_strsplit_space(char const *s)
{
	int		words;
	char	*start;
	char	**result;

	words = count_words((char *)s);
	if (!(result = (char **)malloc(sizeof(char *) * (words + 1))))
		return (NULL);
	start = (char *)s;
	while (*s)
	{
		if (ft_isspace(*s) == 1)
		{
			if (start != s)
				*(result++) = ft_strsub(start, 0, s - start);
			start = (char *)s + 1;
		}
		++s;
	}
	if (start != s)
		*(result++) = ft_strsub(start, 0, s - start);
	*result = NULL;
	return (result - words);
}
