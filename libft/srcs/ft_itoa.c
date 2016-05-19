/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/30 16:37:39 by jguthert          #+#    #+#             */
/*   Updated: 2016/01/03 13:35:06 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

static int			ft_nbrlen(int nbr)
{
	int				i;
	unsigned int	tmp;

	i = 1;
	tmp = nbr;
	if (nbr < 0)
	{
		tmp = -nbr;
		i++;
	}
	while (tmp >= 10)
	{
		tmp /= 10;
		i++;
	}
	return (i);
}

char				*ft_itoa(int nbr)
{
	unsigned int	tmp;
	char			*str;
	int				i;

	tmp = nbr;
	i = ft_nbrlen(nbr);
	str = (char*)malloc(sizeof(char) * (i + 1));
	if (str == NULL)
		return (NULL);
	if (nbr < 0)
	{
		tmp = -nbr;
		str[0] = '-';
	}
	str[i--] = '\0';
	if (tmp == 0)
		str[i] = '0';
	while (tmp > 0)
	{
		str[i] = tmp - (tmp / 10 * 10) + '0';
		tmp /= 10;
		i--;
	}
	return (str);
}
