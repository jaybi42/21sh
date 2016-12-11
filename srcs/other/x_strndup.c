/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/12 15:11:08 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/07 21:56:34 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*x_strndup(const char *s1, size_t n)
{
	char	*new_str;

	if (ft_strlen(s1) > n)
	{
		if (!(new_str = x_strnew(n)))
			return (NULL);
		return (new_str = ft_strncpy(new_str, s1, n));
	}
	else
	{
		if (!(new_str = x_strnew(ft_strlen(s1))))
			return (NULL);
		return (new_str = ft_strcpy(new_str, s1));
	}
}
