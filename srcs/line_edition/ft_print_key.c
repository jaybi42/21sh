/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_key.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 14:05:56 by malaine           #+#    #+#             */
/*   Updated: 2017/01/24 20:51:27 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include "shell.h"

void			ft_print_rest(t_line *l)
{
	char	*temp;

	temp = x_strjoin(l->str, &l->buffer[0]);
	if (temp == NULL)
		return ;
	l->str = temp;
	l->final_count = ft_strlen(l->str);
	l->size = l->final_count;
	print(l);
}

static void		ft_print_insert(t_line *l)
{
	if (ft_insertion(l) == 1)
		return ;
	l->final_count = l->count + 1;
	l->size = ft_strlen(l->str);
	print(l);
}

void			ft_print_line(t_line *l)
{
	int		pos_cur;

	pos_cur = l->count;
	ft_home(l);
	do_term("cd");
	l->count = -1;
	while (l->str[++l->count] != '\0')
		ft_putchar(l->str[l->count]);
	ft_home(l);
	l->count = -1;
	while (++l->count < pos_cur)
		ft_putchar(l->str[l->count]);
}

void			ft_print_key(t_line *l)
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
