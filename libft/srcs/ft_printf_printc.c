/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_printc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/16 14:40:50 by agadhgad          #+#    #+#             */
/*   Updated: 2016/01/17 17:31:37 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void			ft_print_s1(char *s, t_flag *f, int *len, int *null)
{
	*null = (s == NULL) ? 1 : 0;
	if (f->preci_b && f->preci <= 0)
		*len = 0;
	else
	{
		if (*null)
			*len = 6;
		else
		{
			if (f->preci_b)
				*len = (((unsigned int)f->preci > ft_strlen(s)) ?
						ft_strlen(s) : f->preci);
			else
				*len = ft_strlen(s);
		}
	}
}

int					ft_print_s(char *s, t_flag *f)
{
	int len;
	int null;

	ft_print_s1(s, f, &len, &null);
	if (f->mini && !f->less_b)
	{
		if (f->zero_b)
			ft_putc_r(f->mini - len, '0');
		else
			ft_putc_r(f->mini - len, ' ');
	}
	ft_putns(((null) ? "(null)" : s), len);
	if (f->mini && f->less_b)
		ft_putc_r(f->mini - len, ' ');
	return (1);
}

int					ft_print_char(char c, t_flag *f)
{
	if (f->mini && !f->less_b)
	{
		if (f->zero_b)
			ft_putc_r(f->mini - 1, '0');
		else
			ft_putc_r(f->mini - 1, ' ');
	}
	ft_putc(c);
	if (f->mini && f->less_b)
		ft_putc_r(f->mini - 1, ' ');
	return (1);
}

static void			ft_print_ws1(wchar_t *s, t_flag *f, int *len, int *null)
{
	*null = (s == NULL) ? 1 : 0;
	if (f->preci_b && f->preci <= 0)
		*len = 0;
	else
	{
		if (*null)
			*len = 6;
		else
		{
			if (f->preci_b)
				*len = (((unsigned int)f->preci > ft_wwstrlen(s)) ?
						ft_wwstrlen(s) : f->preci);
			else
				*len = ft_wwstrlen(s);
		}
	}
}

int					ft_print_ws(wchar_t *s, t_flag *f)
{
	int len;
	int null;

	ft_print_ws1(s, f, &len, &null);
	if (f->mini && !f->less_b)
	{
		if (f->zero_b)
			ft_putc_r(f->mini - len, '0');
		else
			ft_putc_r(f->mini - len + ((len == 4) ? 1 : 0), ' ');
	}
	ft_putnws((null) ? L"(null)" : s, len);
	if (f->mini && f->less_b)
		ft_putc_r(f->mini - len, ' ');
	return (1);
}
