/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cut_cpy_paste.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 17:51:23 by malaine           #+#    #+#             */
/*   Updated: 2016/11/05 15:47:03 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

void    ft_home_2(t_line *l)
{
    l->nbline = (l->count + l->sizeprompt) / l->largeur;
    do_goto("ch", 0, l->sizeprompt);
    if (l->nbline != 0)
        do_goto("UP", l->nbline, l->nbline);
    l->count = 0;
}

void		print(t_line *l)
{
	int a;

	a = -1;
	ft_home_2(l);
	do_term("cd");
	while (l->str[++a] != '\0')
	{
		ft_putchar(l->str[a]);
		if (l->count != 0 && (l->count + l->sizeprompt + 1) % l->largeur == 0)
			do_term("do");
		l->count++;
	}
	ft_home_2(l);
	while (l->count < l->final_count)
	{
		if (l->count != 0 && (l->count + l->sizeprompt + 1) % l->largeur == 0)
            do_term("do");
		else
			do_term("nd");
        l->count++;
	}
}

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
		l->size = ft_strlen(l->str);
		l->final_count = l->count + ft_strlen(l->strcpy);
		print(l);
	}
}
