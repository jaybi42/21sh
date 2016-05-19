/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ulltoa_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 21:30:56 by jguthert          #+#    #+#             */
/*   Updated: 2016/01/03 13:50:46 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>

static void	get_char_base(unsigned long long nbr, unsigned int base, char *buff,
							int *i)
{
	char const tab_base[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if (nbr >= base)
		get_char_base(nbr / base, base, buff, i);
	buff[(*i)++] = tab_base[nbr % base];
}

char		*ft_ulltoa_base(uint64_t nbr, int base, char *buff)
{
	int				i;

	if (base < 2 || base > 36)
		return (NULL);
	i = 0;
	get_char_base(nbr, (unsigned int)base, buff, &i);
	buff[i] = '\0';
	return (buff);
}
