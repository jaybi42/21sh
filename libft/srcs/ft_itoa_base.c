/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 21:30:56 by jguthert          #+#    #+#             */
/*   Updated: 2016/01/03 13:35:40 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

static void	get_char_base(unsigned int nbr, unsigned int base, char *str,
							int *i)
{
	char const tab_base[] = "0123456789ABCDEF";

	if (nbr >= base)
		get_char_base(nbr / base, base, str, i);
	str[(*i)++] = tab_base[nbr % base];
}

char		*ft_itoa_base(int nbr, int base)
{
	char			*str;
	int				i;

	if (base < 2 || base > 16)
		return (NULL);
	if (!(str = (char*)malloc(sizeof(char) * (32 + 1))))
		return (NULL);
	i = 0;
	if (nbr < 0 && base == 10)
	{
		str[i++] = '-';
		get_char_base((unsigned int)-nbr, (unsigned int)base, str, &i);
	}
	else
		get_char_base((unsigned int)nbr, (unsigned int)base, str, &i);
	str[i] = '\0';
	return (str);
}
