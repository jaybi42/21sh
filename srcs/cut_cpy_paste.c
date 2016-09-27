/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cut_cpy_paste.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 17:51:23 by malaine           #+#    #+#             */
/*   Updated: 2016/09/27 16:48:06 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

void		ft_cut(t_line *l)
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

void		ft_paste(t_line *l)
{
	char	*sauv;
	char	*sauv2;
	int		var;

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
		ft_paste_print(l, var);
	}
}

void		ft_putstr_count(t_line *l)
{
	int		a;

	a = 0;
	while (l->str[a] != '\0')
	{
		ft_putchar(l->str[a]);
		a++;
		l->count++;
	}

}

void		ft_paste_print(t_line *l, int var)
{
	l->size = ft_strlen(l->str);
	ft_home(l);
	do_term("cd");
	ft_putstr_count(l);
	ft_home(l);
	while(l->count < var)
		go_down(l);
}
