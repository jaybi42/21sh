/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_key_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/02 11:04:35 by malaine           #+#    #+#             */
/*   Updated: 2016/11/29 14:13:14 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

void		ft_ctrl_a(t_line *l)
{
	ft_home(l);
}

void		ft_ctrl_e(t_line *l)
{
	ft_end(l);
}

void		ft_left(t_line *l)
{
	if (l->count != 0)
		go_up(l);
}

void		ft_right(t_line *l)
{
	if (l->count < l->size)
		go_down(l);
}
