/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_term.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/08 15:14:45 by jguthert          #+#    #+#             */
/*   Updated: 2016/06/08 15:57:49 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include <stdlib.h>
#include <term.h>
#include <curses.h>
/*
int					init_term(void)
{
	char			*name_term;
	struct termios	term;

	if ((name_term = getenv("TERM")) == NULL)
	{
		ft_putendl_fd("Env inexistant", 2);
		return (-1);
	}
	if (tgetent(NULL, name_term) == ERR)
		return (-1);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		return (-1);
	do_term("ti");
	do_term("cl");
	return (0);
}*/
