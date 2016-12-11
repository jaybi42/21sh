/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_editline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/11 15:58:51 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/11 16:05:54 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <unistd.h>
#include "edit_line.h"

void	debug_editline(t_line *l)
{
	do_term("sc");
	do_goto("DO", 0, 5);
	do_term("cd");
	do_goto("DO", 0, 5);
	ft_putstr("count = ");
	ft_putnbr(l->count);
	ft_putstr(", largeur = ");
	ft_putnbr(l->largeur);
	ft_putstr(", size = ");
	ft_putnbr(l->size);
	ft_putstr(", sizeprompt = ");
	ft_putnbr(l->sizeprompt);
	ft_putstr("\nSTR = ");
	ft_putstr(l->str);
	do_term("rc");
}
