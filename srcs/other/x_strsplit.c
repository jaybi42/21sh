/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/30 21:37:53 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/10 20:03:34 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>

char		*x_strsub(char const *s, unsigned int start, size_t len)
{
	char	*str;
	int		i;

	str = NULL;
	if (s && (int)len >= 0)
	{
		i = 0;
		if (!(str = (char *)xmalloc(sizeof(char) * (len + 1))))
			return (NULL);
		str[len] = '\0';
		while (len--)
			str[i++] = s[start++];
	}
	return (str);
}

static int	count_words(char *s, char c)
{
	int		words;

	while (*s && *s == c)
		++s;
	words = (*s) ? 1 : 0;
	while (*s)
	{
		if (*s == c && s[1] && s[1] != c)
			++words;
		++s;
	}
	return (words);
}

char		**x_strsplit(char const *s, char c)
{
	int		words;
	char	*start;
	char	**result;

	words = count_words((char *)s, c);
	if (!(result = (char **)xmalloc(sizeof(char *) * (words + 1))))
		return (NULL);
	start = (char *)s;
	while (*s)
	{
		if (*s == c)
		{
			if (start != s)
				*(result++) = x_strsub(start, 0, s - start);
			start = (char *)s + 1;
		}
		++s;
	}
	if (start != s)
		*(result++) = x_strsub(start, 0, s - start);
	*result = NULL;
	return (result - words);
}
