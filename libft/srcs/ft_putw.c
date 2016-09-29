/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/16 14:20:14 by agadhgad          #+#    #+#             */
/*   Updated: 2016/01/16 18:55:21 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t			ft_wwlen(wchar_t c)
{
	if (c < 0x80)
	{
		return (1);
	}
	else if (c < 0x800)
	{
		return (2);
	}
	else if (c < 0x10000)
	{
		return (3);
	}
	else if (c < 0x200000)
	{
		return (4);
	}
	return (0);
}

size_t			ft_wwstrlen(wchar_t *s)
{
	size_t i;
	size_t tall;

	i = 0;
	tall = 0;
	if (s != NULL)
	{
		while (s[i])
		{
			tall += ft_wwlen(s[i]);
			i++;
		}
	}
	return (tall);
}

size_t			ft_wstrlen(wchar_t *s)
{
	size_t i;

	i = 0;
	if (s != NULL)
	{
		while (s[i])
			i++;
	}
	return (i);
}

void			ft_putwchar(unsigned int c)
{
	if (c < 0x80)
	{
		ft_putc(c);
	}
	else if (c < 0x800)
	{
		ft_putc(0xC0 | (c >> 6));
		ft_putc(0x80 | (c & 0x3F));
	}
	else if (c < 0x10000)
	{
		ft_putc(0xE0 | (c >> 12));
		ft_putc(0x80 | ((c >> 6) & 0x3F));
		ft_putc(0x80 | (c & 0x3F));
	}
	else if (c < 0x200000)
	{
		ft_putc(0xF0 | (c >> 18));
		ft_putc(0x80 | ((c >> 12) & 0x3F));
		ft_putc(0x80 | ((c >> 6) & 0x3F));
		ft_putc(0x80 | (c & 0x3F));
	}
}
