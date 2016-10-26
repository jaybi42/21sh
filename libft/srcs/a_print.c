/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_printg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/26 20:47:42 by agadhgad          #+#    #+#             */
/*   Updated: 2016/04/01 18:29:30 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "acurse.h"
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

static int			ft_findtype(char type, va_list pa, t_flag *flag)
{
	t_type t;

	if ((find_dioux(type, pa, flag, &t)))
		return (0);
	if ((find_scp(type, pa, flag, &t)))
		return (0);
	if (find_bonus(type) == 1)
		return (0);
	return (1);
}

static const char	*ft_gestion(const char *restrict format, va_list pa)
{
	int		num;
	t_flag	flag;

	ft_initflag(&flag);
	while ((ft_findtype(*format, pa, &flag) > 0))
	{
		num = ft_findflag(format, &flag, pa);
		if (num == 0)
		{
			ft_print_char(*format, &flag);
			return (format);
		}
		else
			format += num;
	}
	return (format);
}

int					a_printf(const char *restrict format, ...)
{
	va_list pa;

	if (format == NULL)
		return (0);
	ret_val(0, 1);
	writex(NULL, 0, -1);
	va_start(pa, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			format = ft_gestion(format, pa);
		}
		else if (*format == '{')
			format += ft_findbonus(format);
		else
			ft_putc(*format);
		(*format) ? format++ : 0;
	}
	va_end(pa);
	writex(NULL, 0, 1);
	return (ret_val(0, 0));
}

int					a_printg(int x, int y, const char *restrict format, ...)
{
	va_list pa;

	tputs(tgoto(tgetstr("cm", NULL), x, y), 1, a_print);
	if (format == NULL)
		return (0);
	ret_val(0, 1);
	writex(NULL, 0, -1);
	va_start(pa, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			format = ft_gestion(format, pa);
		}
		else if (*format == '{')
			format += ft_findbonus(format);
		else
			ft_putc(*format);
		(*format) ? format++ : 0;
	}
	va_end(pa);
	writex(NULL, 0, 1);
	return (ret_val(0, 0));
}
