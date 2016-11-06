/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 16:00:51 by malaine           #+#    #+#             */
/*   Updated: 2016/11/06 16:07:46 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <sys/ioctl.h>

void		reset_line()
{
    struct winsize	w;
	t_line			*l;

	l = g_line;
    ioctl(0, TIOCGWINSZ, &w);
    l->largeur = w.ws_col;
    l->hauteur = w.ws_row;
    l->sizeprompt = l->sizeprompt + 3;
    l->line1 = l->largeur - l->sizeprompt;
	ft_strdel(&l->str);
    l->str = ft_strdup("\0");
    l->tmp = 0;
    l->size = 0;
    l->count = 0;
    l->nbline = 0;
	if (l->strcpy != NULL)
		ft_strdel(&l->strcpy);
    l->hist_pos = -1;
	if (l->oldstr != NULL)
		ft_strdel(&l->oldstr);
	if (l->search != NULL)
		ft_strdel(&l->search);
	if (l->ans != NULL)
		ft_strdel(&l->ans);
}
