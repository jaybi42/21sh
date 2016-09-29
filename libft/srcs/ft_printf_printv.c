/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_printv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/16 14:38:14 by agadhgad          #+#    #+#             */
/*   Updated: 2016/03/07 16:57:00 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int				ft_print_wc(unsigned int c, t_flag *f)
{
	if (f->mini > 0 && !f->less_b)
	{
		if (f->zero_b)
			ft_putc_r(f->mini - 1, '0');
		else
			ft_putc_r(f->mini - 1, ' ');
	}
	ft_putwchar(c);
	if (f->mini > 0 && f->less_b)
	{
		ft_putc_r(f->mini - 1, ' ');
	}
	return (1);
}

static void		ft_print_valu1(unsigned long long n, t_flag *f, int b, int *len)
{
	if (f->less_b && f->diese_b && (b == 8 || b == 16) && !f->p_b)
		f->mini += (b == 8) ? 1 : 2;
	if (f->diese_b && (b == 8 || b == 16) && !f->p_b && f->zero_b)
		f->mini -= (b == 8) ? 1 : 2;
	if (f->zero_b && f->preci_b && (f->preci < f->mini))
		f->zero_b = 0;
	if (f->diese_b && n == 0 && !(b == 8 && f->preci_b && f->preci <= 0))
	{
		f->diese_b = 0;
		if (f->p_b == 1)
			f->diese_b = 1;
	}
	if ((b == 16 && !f->preci_b && n == 0 && f->zero_b) && f->diese_b)
		*len += 2;
}

int				ft_print_valu(unsigned long long int n, t_flag *f, int base)
{
	int		len;
	int		upper;
	char	*s;

	upper = (base == 17) ? 0 : 32;
	base = (base == 17) ? 16 : base;
	len = ft_strlen(ft_itoa_base(n, base, upper));
	if (f->preci_b && f->preci <= 0)
		f->mini += (f->mini > 0) ? 1 : 0;
	ft_print_valu1(n, f, base, &len);
	if (f->mini > 0 && f->zero_b == 1)
		f->preci = (f->mini > f->preci) ? f->mini : f->preci;
	(f->mini > 0 && !f->less_b && !f->zero_b) ? ft_miniu(len, f, base) : 0;
	if ((f->sp_b || f->sign_b) && base != 16 && base != 8)
		(f->sign_b) ? ft_putc('+') : 0;
	(f->diese_b) ? ft_putnorme(base, f, upper) : 0;
	(f->preci) ? ft_putc_r(f->preci - len, '0') : 0;
	if (!(f->preci_b && f->preci <= 0 && n == 0))
	{
		ft_puts((s = ft_itoa_base(n, base, upper)));
		free(s);
	}
	if (f->mini && f->less_b && !f->zero_b)
		ft_miniu(len, f, base);
	return (1);
}

static void		ft_print_val1(long long int *n, t_flag *f, int *p, int *len)
{
	char *s;

	if (f->zero_b && f->preci_b && (f->preci < f->mini))
		f->zero_b = 0;
	*p = (f->preci_b && *n < 0) ? 1 : 0;
	if (f->zero_b)
		f->preci = (f->mini > f->preci) ? f->mini : f->preci + *p;
	else
		f->preci = f->preci + *p;
	s = ft_litoa(*n);
	*len = ft_strlen(s);
	if (f->preci_b && f->preci <= 0)
		f->mini += (f->mini > 0) ? 1 : 0;
	(f->mini > 0 && !f->less_b && !f->zero_b) ? ft_mini(*len, f, n) : 0;
	if ((f->sp_b || f->sign_b) && *n >= 0)
	{
		if (*n == 0 || f->zero_b)
			*len = *len + 1;
		if (f->sp_b)
			ft_putc(' ');
		else if (f->sign_b)
			ft_putc('+');
	}
	ft_strdel(&s);
}

int				ft_print_val(long long int n, t_flag *f)
{
	int		p;
	int		len;
	char	*s;

	ft_print_val1(&n, f, &p, &len);
	if (n < 0)
	{
		ft_putc('-');
		n *= -1;
	}
	(f->preci) ? ft_putc_r(f->preci - len, '0') : 0;
	if (!(n == 0 && f->preci_b && f->preci <= 0))
	{
		ft_puts((s = ft_litoa(n)));
		ft_strdel(&s);
	}
	if (f->less_b && f->mini && f->sign_b)
		len -= 1;
	if (f->mini && f->less_b && !f->zero_b)
		ft_mini(len, f, &n);
	return (1);
}
