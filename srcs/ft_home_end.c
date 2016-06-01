/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_home_end.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 17:35:38 by malaine           #+#    #+#             */
/*   Updated: 2016/05/31 14:51:12 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <curses.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <term.h>
#include <sys/ioctl.h>

void	ft_home(t_line *l)
{
	l->nbline = (l->count + l->sizeprompt) / l->largeur;
	do_goto("ch", 0, l->sizeprompt);
	if (l->nbline != 0)
		do_goto("UP", l->nbline, l->nbline);
	l->count = 0;
}

void	ft_end(t_line *l)
{
	while (l->count < l->size)
		ft_go_down_bitch(l);
}
