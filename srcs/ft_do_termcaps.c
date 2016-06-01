/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_do_termcaps.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/31 14:25:24 by malaine           #+#    #+#             */
/*   Updated: 2016/05/31 14:53:36 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <curses.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <term.h>
#include <stdlib.h>

void	do_term(char *str)
{
	tputs(tgetstr(str, NULL), 1, putchar_s);
}

void	do_goto(char *str, int val1, int val2)
{
	tputs(tgoto(tgetstr(str, NULL), val1, val2), 1, putchar_s);
}
