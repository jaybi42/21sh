/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_space.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/02 17:20:16 by jguthert          #+#    #+#             */
/*   Updated: 2016/06/06 19:05:49 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

void	check_is_space_l(t_line *l)
{
	while (l->str[l->count - 1] == ' ')
		go_up(l);
}

void	check_space_l(t_line *l)
{
	while (l->str[l->count - 1] != ' ' && l->count != 0)
		go_up(l);
}

void	check_space_r(t_line *l)
{
	while (l->str[l->count] != ' ' && l->str[l->count] != '\0')
		go_down(l);
}

void	check_is_space_r(t_line *l)
{
	while (l->str[l->count] == ' ')
		go_down(l);
}
