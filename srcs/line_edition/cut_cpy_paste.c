/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cut_cpy_paste.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 17:51:23 by malaine           #+#    #+#             */
/*   Updated: 2016/11/26 20:31:19 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

void		print(t_line *l)
{
	int a;

	a = -1;
	ft_home(l);
	do_term("cd");
	while (l->str[++a] != '\0')
	{
		ft_putchar(l->str[a]);
		if (l->largeur != 0 && l->count != 0
		&& (l->count + l->sizeprompt + 1) % l->largeur == 0)
			do_term("do");
		l->count++;
	}
	ft_home(l);
	while (l->count < l->final_count)
	{
		if (l->largeur != 0 && l->count != 0
		&& (l->count + l->sizeprompt + 1) % l->largeur == 0)
			do_term("do");
		else
			do_term("nd");
		l->count++;
	}
}

void		ft_cut(t_line *l)
{
	char	*tmp;

	do_term("cd");
	if (l->strcpy != NULL)
		ft_strdel(&l->strcpy);
	l->strcpy = ft_strsub(l->str, l->count, l->size);
	if (l->strcpy == NULL)
		return ;
	if (l->count != 0)
		tmp = ft_strsub(l->str, 0, l->count);
	else
		tmp = ft_strdup("\0");
	if (tmp == NULL)
		return ;
	//ft_strdel(&l->str);
	l->str = tmp;
	l->size = ft_strlen(l->str);
}

static void	do_join_paste(char *tmp, char *tmp2, t_line *l)
{
	char	*str;
	char	*str2;

	str = ft_strjoin(tmp, l->strcpy);
	if (str == NULL)
		return ;
	str2 = ft_strjoin(str, tmp2);
	ft_strdel(&str);
	if (str2 == NULL)
		return ;
	//ft_strdel(&l->str);
	l->str = str2;
}

void		ft_paste(t_line *l)
{
	char	*tmp;
	char	*tmp2;

	if (l->strcpy != NULL)
	{
		tmp = ft_strsub(l->str, 0, l->count);
		if (tmp == NULL)
			return ;
		tmp2 = ft_strsub(l->str, l->count, l->size);
		if (tmp2 == NULL)
			return ;
		do_join_paste(tmp, tmp2, l);
		ft_strdel(&tmp);
		ft_strdel(&tmp2);
		l->size = ft_strlen(l->str);
		l->final_count = l->count + ft_strlen(l->strcpy);
		print(l);
	}
}
