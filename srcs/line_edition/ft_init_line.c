/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 14:13:34 by malaine           #+#    #+#             */
/*   Updated: 2016/12/24 02:01:55 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "edit_line.h"
#include <sys/ioctl.h>

void	ft_init_line(t_line *l, t_ftl_root *hist)
{
	struct winsize w;

	ioctl(0, TIOCGWINSZ, &w);
	l->largeur = w.ws_col;
	l->hauteur = w.ws_row;
	l->sizeprompt = l->sizeprompt + 3;
	l->line1 = l->largeur - l->sizeprompt;
	l->str = x_strdup("");
	l->tmp = 0;
	l->size = 0;
	l->start_print = -1;
	l->count = 0;
	l->nbline = 0;
	l->hist = hist;
	l->hist_size = 30;
	l->hist_pos = -1;
	l->oldstr = NULL;
	l->search = NULL;
	l->ans = NULL;
}

void	ft_init_line_heredoc(t_line *l)
{
	if (g_line != NULL)
		l->sizeprompt = g_line->sizeprompt;
	l->size = 0;
	l->str = x_strdup("");
	l->count = 0;
}
