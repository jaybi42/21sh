/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/16 14:24:10 by agadhgad          #+#    #+#             */
/*   Updated: 2016/04/01 18:46:57 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void			ft_putc_r(int q, char c)
{
	int i;

	i = 0;
	while (i++ < q)
		ft_putc(c);
}

int				ret_val(int val, int first)
{
	static int a;

	if (first)
		a = 0;
	a += val;
	return (a);
}

void			ft_putc(char c)
{
	writex(&c, 1, 0);
	ret_val(1, 0);
}

void			ft_putns(char *s, int n)
{
	if (s != NULL)
		writex(s, n, 0);
	ret_val(n, 0);
}

void			ft_puts(char *s)
{
	size_t i;

	if (s != NULL)
	{
		i = ft_strlen(s);
		writex(s, i, 0);
		ret_val(i, 0);
	}
}
