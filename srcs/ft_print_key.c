/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_key.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 14:05:56 by malaine           #+#    #+#             */
/*   Updated: 2016/09/13 15:12:27 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"


static void		ft_print_first(t_line *l)
{
	l->count++;
	l->str = ft_strdup(&l->buffer[0]);
	ft_putstr(l->str);
	l->size++;
}

static void		ft_print_rest(t_line *l)
{
	l->count++;
	l->str = ft_strjoin(l->str, &l->buffer[0]);
	ft_putstr(&l->buffer[0]);
	if ((l->count + l->sizeprompt) % l->largeur == 0)
		do_term("do");
	l->size++;
}

static void		ft_print_insert(t_line *l)
{
	l->str = ft_insertion(l->count, l->str, l->size, l->buffer[0]);
	do_term("cd");
	l->size++;
	ft_putchar(l->str[l->count++]);
	if ((l->count + l->sizeprompt) % l->largeur == 0)
		do_term("do");
	do_term("sc");
	ft_putstr(l->str + l->count);
	do_term("rc");
}

void			ft_print_key(t_line *l)
{
	if (l->str != NULL && l->count == l->size)
		ft_print_rest(l);
	else if (l->str == NULL)
		ft_print_first(l);
	else
		ft_print_insert(l);
}
/*

static void			go_to_cursor(t_line *l)
{
	int		nbline;

	nbline = (l->count + l->sizeprompt) / l->largeur;
	ft_putnbr(nbline);
	if (nbline != 0)
	{
		do_goto("DO", 0, nbline);
		do_goto("ts", 0 , 0);
		do_goto("RI", (l->count + l->sizeprompt) % nbline, 0);
		l->nbline = nbline;
	}
	else
		do_goto("RI", 0, l->count);
}

void			ft_print_line(t_line *l)
{
	int		n;

	n = l->count;
	ft_home(l);
	do_term("cd");
	ft_putstr(l->str);
	ft_home(l);
	l->count = n;
	go_to_cursor(l);
}

static int		edit_str(t_line *l)
{
	char	*temp;

	if (l->str == NULL)
		l->str = ft_strdup(&l->buffer[0]);
	else
	{
		if (l->count != l->size)
			temp = ft_insertion(l->count, l->str, l->size, l->buffer[0]);
		else
			temp = ft_strjoin(l->str, &l->buffer[0]);
		if (temp == NULL)
			return (0);
		ft_strdel(&(l->str));
		l->str = temp;
	}
	return (1);
}

void			ft_print_key(t_line *l)
{
	if (!edit_str(l))
		return ;
	l->count++;
	l->size++;
	ft_print_line(l);
}
*/
