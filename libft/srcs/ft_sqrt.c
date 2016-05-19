/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 18:11:42 by jguthert          #+#    #+#             */
/*   Updated: 2015/12/12 15:02:43 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_sqrt(int nbr)
{
	int sqrt;
	int max;
	int min;
	int tmp;

	if (nbr < 2)
		return (nbr);
	min = 0;
	if (nbr / 2 > 46340)
		max = 46341;
	else
		max = nbr / 2;
	while (max - min > 1)
	{
		sqrt = (max + min) / 2;
		tmp = ft_power(sqrt, 2);
		if (tmp == nbr)
			return (sqrt);
		else if (tmp > nbr)
			max = sqrt;
		else
			min = sqrt;
	}
	return (nbr);
}
