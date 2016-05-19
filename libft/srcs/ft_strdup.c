/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/03 15:59:10 by jguthert          #+#    #+#             */
/*   Updated: 2015/12/12 16:34:56 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(char const *src)
{
	char	*new_str;

	if (!(new_str = ft_strnew(ft_strlen(src))))
		return (NULL);
	new_str = ft_strcpy(new_str, src);
	return (new_str);
}
