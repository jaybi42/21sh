/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_term.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/12 16:38:33 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/18 13:26:32 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <stdlib.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>

/*
** TERMIOS STRUCT
** {
** tcflag_t		c_iflag
** tcflag_t		c_oflag
** tcflag_t		c_cflag
** tcflag_t		c_lflag
** cc_t			c_cc[NCCS]
** speed_t		c_ispeed
** speed_t		c_ospeed
** }
*/

int			reset_term(void)
{
	TERM	term;

	if (do_termcap("cl") == 1 || do_termcap("ve") == 1)
		return (1);
	if (tcgetattr(0, &term) == -1)
		return (print_error("ft_select", 3));
	if (tcsetattr(0, 0, &term) == -1)
		return (print_error("ft_select", 2));
	return (0);
}

int			init_term(void)
{
	char	*term_name;
	TERM	term;

	term_name = getenv("TERM");
	if (term_name == NULL)
		return (print_error("ft_select", 0));
	if (tgetent(NULL, term_name) != 1)
		return (print_error("ft_select", 1));
	if (tcgetattr(0, &term) == -1)
		return (print_error("ft_select", 3));
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (do_termcap("vi") == 1)
		return (1);
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		return (print_error("ft_select", 2));
	return (0);
}
