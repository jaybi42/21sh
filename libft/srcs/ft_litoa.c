/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_litoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/16 13:46:24 by agadhgad          #+#    #+#             */
/*   Updated: 2016/01/22 12:29:56 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		*ret_zero(char *s, char *s2)
{
	s[0] = '0';
	s[1] = '\0';
	ft_strdel(&s2);
	return (s);
}

static void		ft_init(char **s, char **s2, int *i, int *y)
{
	*s2 = (char *)malloc(sizeof(char *) * (64));
	*s = (char *)malloc(sizeof(char *) * (64));
	*i = 0;
	*y = 0;
}

char			*ft_itoa_base(unsigned long long nb, char base, int size)
{
	char	*s;
	char	*s2;
	int		i;
	int		y;

	ft_init(&s, &s2, &i, &y);
	if (nb == 0)
		return (ret_zero(s, s2));
	while (nb > 0)
	{
		s2[i] = nb % base + '0';
		s2[i] += (s2[i] > '9') ? 7 + size : 0;
		nb /= base;
		i++;
	}
	i--;
	while (i >= 0)
	{
		s[y] = s2[i];
		i--;
		y++;
	}
	s[y] = '\0';
	ft_strdel(&s2);
	return (s);
}

char			*ft_litoa(unsigned long long nb)
{
	int		i;
	int		y;
	char	*s;
	char	*s2;

	ft_init(&s, &s2, &i, &y);
	if (nb == 0)
		return (ret_zero(s, s2));
	while (nb > 0)
	{
		s2[i] = nb % 10 + '0';
		nb /= 10;
		i++;
	}
	i--;
	while (i >= 0)
	{
		s[y] = s2[i];
		i--;
		y++;
	}
	s[y] = '\0';
	ft_strdel(&s2);
	return (s);
}
