/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/16 14:32:35 by agadhgad          #+#    #+#             */
/*   Updated: 2016/01/17 18:45:45 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void				ft_initflag(t_flag *flag)
{
	flag->sp_b = 0;
	flag->sign_b = 0;
	flag->preci = 0;
	flag->preci_b = 0;
	flag->diese_b = 0;
	flag->less_b = 0;
	flag->mini = 0;
	flag->modif[0] = 0;
	flag->modif[1] = 0;
	flag->zero_b = 0;
	flag->p_b = 0;
	flag->bonus_base = 0;
}

void				ft_putnorme(int base, t_flag *f, int upper)
{
	if (base == 8)
	{
		ft_putc('0');
		f->preci--;
		f->mini--;
	}
	else if (base == 16)
	{
		if (upper == 32)
			ft_puts("0x");
		else
			ft_puts("0X");
		f->mini -= 2;
	}
}

void				ft_mini(int longueur, t_flag *f, long long int *nb)
{
	int len;
	int i;

	len = 0;
	if (*nb < 0 && f->sp_b)
		len--;
	if (((f->sp_b || f->sign_b) && *nb > 0))
		len++;
	if (longueur > f->preci)
		len += longueur;
	else
		len += f->preci;
	if (f->less_b && f->mini != 0 && f->sign_b &&
			ft_strlen(ft_litoa(*nb)) < (unsigned)(f->mini - len) && !f->preci_b)
		i = 1;
	else
		i = 0;
	while (i < f->mini - len)
	{
		ft_putc(' ');
		i++;
	}
}

void				ft_miniu(int longueur, t_flag *f, int b)
{
	int len;
	int i;

	len = 0;
	if (f->sp_b)
		len--;
	if ((f->sp_b || f->sign_b))
		len++;
	if (f->diese_b && !(f->less_b && f->p_b))
		len += ((b == 16) ? 2 : 1);
	if (longueur > f->preci)
		len += longueur;
	else
		len += f->preci;
	i = 0;
	while (i < f->mini - len)
	{
		ft_putc(' ');
		i++;
	}
}

void				ft_putnws(wchar_t *s, size_t len)
{
	int		i;
	size_t	tall;

	i = 0;
	tall = 0;
	while (tall < len)
	{
		if (tall + ft_wwlen(s[i]) <= len)
			ft_putwchar(s[i]);
		tall += ft_wwlen(s[i]);
		i++;
	}
}
