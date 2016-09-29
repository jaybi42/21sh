/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_find2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/16 14:14:40 by agadhgad          #+#    #+#             */
/*   Updated: 2016/01/16 18:24:27 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			find_ubase(char type, t_flag *f)
{
	if (type == 'b')
		return (2);
	if (f->bonus_base != 0)
		return (f->bonus_base);
	if (type == 'x')
		return (16);
	if (type == 'X')
		return (17);
	if (type == 'o' || type == 'O')
		return (8);
	if (type == 'u' || type == 'U')
		return (10);
	return (0);
}

int			tbase(char type)
{
	if (type == 'd' || type == 'i')
		return (1);
	if (type == 'D')
		return (2);
	return (0);
}

int			utbase(char type)
{
	if (type == 'x' || type == 'o' || type == 'u' || type == 'X' || type == 'b')
		return (1);
	if (type == 'O' || type == 'U')
		return (2);
	return (0);
}

static int	find_scp1(char type, va_list pa, t_flag *flag, t_type *t)
{
	if (type == 'c' && flag->modif[0] != 'l')
	{
		t->c = va_arg(pa, int);
		return (ft_print_char(t->c, flag));
	}
	else if (type == 'C' || (type == 'c' && flag->modif[0] == 'l'))
	{
		t->u = va_arg(pa, unsigned int);
		return (ft_print_wc(t->u, flag));
	}
	else if (type == 's' && flag->modif[0] != 'l')
	{
		flag->sp_b = 0;
		t->str = va_arg(pa, char *);
		return (ft_print_s(t->str, flag));
	}
	else if (type == 'S' || (type == 's' && flag->modif[0] == 'l'))
	{
		t->wstr = va_arg(pa, wchar_t *);
		return (ft_print_ws(t->wstr, flag));
	}
	return (0);
}

int			find_scp(char type, va_list pa, t_flag *flag, t_type *t)
{
	if (find_scp1(type, pa, flag, t))
		return (1);
	else if (type == 'p')
	{
		t->lu = va_arg(pa, unsigned long int);
		flag->diese_b = 1;
		flag->p_b = 1;
		return (ft_print_valu(t->lu, flag, 16));
	}
	else if (type == '%')
		return (ft_print_char('%', flag));
	else if (type == '\0')
		return (1);
	return (0);
}
