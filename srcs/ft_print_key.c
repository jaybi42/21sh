/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_key.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 14:05:56 by malaine           #+#    #+#             */
/*   Updated: 2016/06/06 19:11:14 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

void	ft_print_first(t_line *l)
{
	l->count++;
	l->str = ft_strdup(&l->buffer[0]);
	ft_putstr(l->str);
	l->size++;
}

void	ft_print_rest(t_line *l)
{
	l->count++;
	l->str = ft_strjoin(l->str, &l->buffer[0]);
	ft_putstr(&l->buffer[0]);
	if ((l->count + l->sizeprompt) % l->largeur == 0)
		do_term("do");
	l->size++;
}

void	ft_print_insert(t_line *l)
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

void	ft_print_key(t_line *l)
{
	if (l->str != NULL && l->count == l->size)
		ft_print_rest(l);
	else if (l->str == NULL)
		ft_print_first(l);
	else
		ft_print_insert(l);
}
