/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_key.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 14:05:56 by malaine           #+#    #+#             */
/*   Updated: 2016/10/26 16:04:58 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

static void ft_print_rest(t_line *l)
{
	char	*temp;

	l->count++;
	temp = ft_strjoin(l->str, &l->buffer[0]);
	if (temp == NULL)
	{
		l->count--;
		return ;
	}
	ft_strdel(&(l->str));
	l->str = temp;
	ft_putstr(&l->buffer[0]);
	if (l->largeur != 0 && ((l->count + l->sizeprompt) % l->largeur == 0))
		do_term("do");
	l->size++;
}

static void	ft_print_insert(t_line *l)
{
	int pos_cur;
	int i;

	i = -1;
	l->str = ft_insertion(l->count, l->str, l->size, l->buffer[0]);
	pos_cur = l->count;
	ft_home(l);
	do_term("cd");
	while(l->str[++i] != '\0')
	{
		ft_putchar(l->str[i]);
		l->count++;
	}
	i = -1;
	while (++i < (l->size - pos_cur))
	{
		do_term("le");
		l->count--;
	}
	l->size++;
	if ((l->size + l->sizeprompt) % l->largeur == 0)
		do_term("nd");
}

void        ft_print_line(t_line *l)
{
    int     pos_cur;

    pos_cur = l->count;
    l->count = -1;
    while(l->str[++l->count] != '\0')
        ft_putchar(l->str[l->count]);
    ft_home(l);
    l->count = -1;
    while (++l->count < pos_cur)
        ft_putchar(l->str[l->count]);
}

void        ft_clean(t_line *l, int a, char *str)
{
    int     pos_cur;
    char    *tmp;

    pos_cur = l->count;
    l->count = a;
    tmp = ft_strdup(l->str);
    l->str = ft_strdup(str);
    ft_home(l);
    do_term("cd");
    l->count = pos_cur;
    l->str = ft_strdup(tmp);
    ft_print_line(l);
}

void		ft_print_key(t_line *l)
{
	if (!ft_isprint(l->buffer[0]))
		return ;
	l->oldstr = NULL;
	if (l->str != NULL && l->count == l->size)
		ft_print_rest(l);
	else
	{
		if (l->str == NULL)
			l->str = ft_strdup(&l->buffer[0]);
		ft_print_insert(l);
	}
}
