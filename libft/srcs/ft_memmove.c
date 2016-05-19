/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 10:34:24 by jguthert          #+#    #+#             */
/*   Updated: 2016/01/03 13:36:15 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_memmove(void *str1, const void *str2, size_t n)
{
	unsigned char	*str;

	if (!(str = (unsigned char*)malloc(sizeof(*str) * n)))
		return (NULL);
	ft_memcpy(str, str2, n);
	ft_memcpy(str1, str, n);
	free(str);
	return (str1);
}
