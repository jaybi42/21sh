/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cut_cpy_paste.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 17:51:23 by malaine           #+#    #+#             */
/*   Updated: 2016/05/31 14:54:27 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <curses.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <term.h>
#include <stdlib.h>

void	ft_cut(t_line *l)
{
	do_term("cd");
	l->strcpy = ft_strsub(l->str, l->count, l->size);
	if (l->count != 0)
	{
		l->str = ft_strsub(l->str, 0, l->count);
		l->size = ft_strlen(l->str);
	}
	else
	{
		l->str = ft_strdup("\0");
		l->size = 0;
	}
}

void	ft_paste(t_line *l)
{
	char	*sauv;
	char	*sauv2;
	int		var;
	int		tmp;

	tmp = l->count;
	if (l->strcpy != NULL)
	{
		if (l->str[0] != '\0')
		{
			sauv2 = ft_strsub(l->str, l->count, l->size);
			sauv = ft_strsub(l->str, 0, l->count);
			l->str = ft_strjoin(sauv, l->strcpy);
			l->str = ft_strjoin(l->str, sauv2);
		}
		else
			l->str = ft_strdup(l->strcpy);
		var = l->count + ft_strlen(l->strcpy);
		ft_paste_print(l, var, tmp);
	}
}

void	ft_paste_print(t_line *l, int var, int tmp)
{
	do_term("ce");
	while (l->str[l->count] != '\0')
	{
		ft_putchar(l->str[l->count]);
		if (l->count + 1 == var)
			do_term("sc");
		l->count++;
	}
	if (tmp < l->size)
	{
		l->count = tmp + ft_strlen(l->strcpy);
		do_term("rc");
	}
	l->size = ft_strlen(l->str);
}
