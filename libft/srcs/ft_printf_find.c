/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_find.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/16 13:18:17 by agadhgad          #+#    #+#             */
/*   Updated: 2016/01/16 19:07:09 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		find_type(char type, t_flag *f, int (*ft)(char))
{
	int t;

	t = ft(type);
	if (t == 1 && f->modif[1] == 'h')
		return (-2);
	if ((t == 1 && f->modif[0] == 'h' && f->modif[1] == 0))
		return (-1);
	if ((t == 1 && f->modif[0] == 0))
		return (0);
	if ((t == 1 && f->modif[0] == 'l' && f->modif[1] == 0)
			|| (t == 2 && f->modif[0] == 0))
		return (1);
	if ((t == 1 && f->modif[1] == 'l')
			|| (t == 2 && f->modif[0] == 'l'))
		return (2);
	if (f->modif[0] == 'j')
		return (3);
	if (f->modif[0] == 'z')
		return (4);
	return (2);
}

static int		find_dioux2(char type, va_list pa, t_flag *f, t_type *t)
{
	int i;

	i = find_type(type, f, utbase);
	if (i == 1)
	{
		t->lu = va_arg(pa, unsigned long int);
		return (ft_print_valu(t->lu, f, find_ubase(type, f)));
	}
	else if (i == 2 || i == 3 || i == 4)
	{
		t->llu = va_arg(pa, unsigned long long int);
		return (ft_print_valu(t->llu, f, find_ubase(type, f)));
	}
	else if (i == 3)
	{
		t->ju = va_arg(pa, uintmax_t);
		return (ft_print_valu(t->ju, f, find_ubase(type, f)));
	}
	else
	{
		t->z = va_arg(pa, size_t);
		return (ft_print_valu(t->z, f, find_ubase(type, f)));
	}
}

static int		find_dioux1(char type, va_list pa, t_flag *f, t_type *t)
{
	int i;

	i = find_type(type, f, tbase);
	if (i <= 0)
	{
		t->n = va_arg(pa, int);
		t->n = (i == -2) ? (char)t->n : t->n;
		t->n = (i == -1) ? (short)t->n : t->n;
		return (ft_print_val(t->n, f));
	}
	else if (i == 1)
	{
		t->ln = va_arg(pa, long int);
		return (ft_print_val(t->ln, f));
	}
	else if (i == 2)
	{
		t->lln = va_arg(pa, long long int);
		return (ft_print_val(t->lln, f));
	}
	else
	{
		t->j = va_arg(pa, intmax_t);
		return (ft_print_val(t->j, f));
	}
}

int				find_dioux(char type, va_list pa, t_flag *f, t_type *t)
{
	int type_indic;

	if (tbase(type) != 0)
		return (find_dioux1(type, pa, f, t));
	if (utbase(type) != 0)
	{
		f->sign_b = 0;
		type_indic = find_type(type, f, utbase);
		if (type_indic <= 0)
		{
			t->u = va_arg(pa, unsigned int);
			if (type_indic == -2)
				t->u = (unsigned char)t->u;
			else if (type_indic == -1)
				t->u = (unsigned short)t->u;
			return (ft_print_valu(t->u, f, find_ubase(type, f)));
		}
		return (find_dioux2(type, pa, f, t));
	}
	return (0);
}
