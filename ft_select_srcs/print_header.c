/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/18 15:02:12 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/18 18:48:40 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static void			p_h(char *str, int col, int color)
{
	move_termcap("ch", col, col);
	ft_putstr_fd("\e[38;5;", 2);
	ft_putnbr_fd(color, 2);
	ft_putchar_fd('m', 2);
	ft_putendl_fd(str, 2);
	ft_putstr_fd("\e[0m", 2);
}

static void			header_basic(int s)
{
	int		c;

	c = 240;
	p_h(" ______ _______ _____ ______ _      ______ _____ _______", s, c++);
	p_h("|  ____|__   __/ ____|  ____| |    |  ____/ ____|__   __|", s, c++);
	p_h("| |__     | | | (___ | |__  | |    | |__ | |       | |", s, c++);
	p_h("|  __|    | |  \\___ \\|  __| | |    |  __|| |       | |", s, c++);
	p_h("| |       | |  ____) | |____| |____| |___| |____   | |", s, c++);
	p_h("|_|       |_| |_____/|______|______|______\\_____|  |_|", s, c++);
	p_h("          ______", s, c++);
	p_h("         |______|", s, c);
}

static void			header_extended(int s)
{
	int		c;

	c = 240;
	p_h("", s, c++);
	p_h("[Arrows]        Navigate in the list", s, c++);
	p_h("[Enter]         Validate selection", s, c++);
	p_h("[Space bar]     Select a file", s, c++);
	p_h("[Delete]        Delete a file", s, c++);
	p_h("", s, c++);
	p_h("[ESC]           Exit the program", s, c++);
}

void				print_header(void)
{
	struct winsize	win;

	ioctl(0, TIOCGWINSZ, &win);
	if (win.ws_col > 102)
	{
		move_termcap("cm", 0, 0);
		header_basic((win.ws_col - 100) / 2);
		move_termcap("cm", 0, 0);
		header_extended((win.ws_col - 100) / 2 + 59);
	}
	else if (win.ws_col > 59)
	{
		move_termcap("cm", 0, 0);
		header_basic((win.ws_col - 57) / 2);
	}
}
