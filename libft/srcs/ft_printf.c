/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/01 16:41:20 by agadhgad          #+#    #+#             */
/*   Updated: 2016/11/27 17:46:17 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int					find_bonus(char type)
{
	time_t		t;
	struct tm	tm;

	if (type == 'k')
	{
		t = time(NULL);
		tm = *localtime(&t);
		ft_printf("%02d/%02d/%02d %02d:%02d:%02d",
				tm.tm_year + 1900, tm.tm_mon + 1,
				tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		return (1);
	}
	return (0);
}

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

int					ft_printf(const char *restrict format, ...)
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
		{
			ft_putc(*format);
		}
		(*format) ? format++ : 0;
	}
	va_end(pa);
	writex(NULL, 0, 1);
	return (ret_val(0, 0));
}
