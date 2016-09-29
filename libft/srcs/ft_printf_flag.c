/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/16 14:09:35 by agadhgad          #+#    #+#             */
/*   Updated: 2016/01/17 19:19:28 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_findflag2(const char *restrict pf, t_flag *f, va_list pa)
{
	int ret;

	ret = 1;
	if (*pf == '.')
	{
		pf++;
		f->preci_b = 1;
		f->preci = 0;
		if (*pf == '*')
		{
			f->preci = va_arg(pa, int);
			f->preci_b = (f->preci < 0) ? 0 : 1;
			return (2);
		}
		while (ft_isdigit(*pf))
		{
			f->preci += *pf - '0';
			f->preci *= 10;
			ret++;
			pf++;
		}
		f->preci /= 10;
		return (ret);
	}
	return (0);
}

static int		ft_findflag1(const char *restrict pf, t_flag *f)
{
	if (*pf == ' ')
	{
		if (!f->sp_b && !f->sign_b)
			f->sp_b = 1;
		return (1);
	}
	else if (*pf == '+')
	{
		if (!f->sign_b)
			f->sign_b = 1;
		if (f->sp_b)
			f->sp_b = 0;
		return (1);
	}
	else if (*pf == '-')
	{
		f->less_b = 1;
		f->zero_b = 0;
		return (1);
	}
	else if (*pf == 'h' || *pf == 'l' || *pf == 'j' || *pf == 'z')
		return (ft_addmodif(pf, f));
	return (0);
}

static int		ft_findflag4b(const char *restrict pf, t_flag *f, va_list pa)
{
	if (*pf == '0')
		f->zero_b = ((f->less_b) ? 0 : 1);
	if (*pf == '*')
	{
		f->mini = va_arg(pa, int);
		return (1);
	}
	return (0);
}

int				ft_findflag4(const char *restrict pf, t_flag *f, va_list pa)
{
	int ret;
	int v;

	if (ft_findflag4b(pf, f, pa))
		return (1);
	else if (ft_isdigit(*pf))
	{
		ret = 1;
		v = f->mini;
		f->mini = 0;
		while (ft_isdigit(*pf))
		{
			f->mini += *pf - '0';
			f->mini *= 10;
			pf++;
			ret++;
		}
		f->mini /= 10;
		if (f->mini < v)
			f->mini = v - 1;
		return (ret - 1);
	}
	return (0);
}

int				ft_findflag(const char *restrict pf, t_flag *f, va_list pa)
{
	int ret;

	if ((ret = ft_findflag1(pf, f)))
		return (ret);
	else if ((ret = ft_findflag2(pf, f, pa)))
		return (ret);
	else if ((ret = ft_findflag4(pf, f, pa)))
		return (ret);
	else if ((ret = ft_findflag3(pf, f)))
		return (ret);
	else if (*pf == '#')
	{
		f->diese_b = 1;
		return (1);
	}
	return (0);
}
