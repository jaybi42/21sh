/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 16:00:51 by malaine           #+#    #+#             */
/*   Updated: 2016/12/19 16:48:22 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <sys/ioctl.h>

void		reset_line(void)
{
	struct winsize	w;
	t_line			*l;

	l = g_line;
	ioctl(0, TIOCGWINSZ, &w);
	l->largeur = w.ws_col;
	l->hauteur = w.ws_row;
	l->sizeprompt = l->sizeprompt + 3;
	l->line1 = l->largeur - l->sizeprompt;
	l->str = x_strdup("");
	l->tmp = 0;
	l->final_count = 0;
	l->size = 0;
	l->count = 0;
	l->nbline = 0;
	l->hist_pos = -1;
	l->oldstr = NULL;
	l->search = NULL;
	l->ans = NULL;
}
