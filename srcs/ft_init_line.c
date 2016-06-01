/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 14:13:34 by malaine           #+#    #+#             */
/*   Updated: 2016/06/01 18:19:19 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <sys/ioctl.h>

void	ft_init_read(t_line *l)
{
	struct winsize w;

	ioctl(0, TIOCGWINSZ, &w);
	l->largeur = w.ws_col;
	l->hauteur = w.ws_row;
	l->sizeprompt = l->sizeprompt + 3;
	l->line1 = l->largeur - l->sizeprompt;
	*av_list = NULL;
	l->str = ft_strdup("\0");
	l->tmp = 0;
	l->size = 0;
	l->count = 0;
	l->strcpy = NULL;
}
