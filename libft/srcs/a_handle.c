/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_handle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/01 00:04:50 by agadhgad          #+#    #+#             */
/*   Updated: 2016/04/01 19:45:11 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "acurse.h"

void	a_resize(int id)
{
	struct winsize s;

	ioctl(0, TIOCGWINSZ, &s);
	a_storage_data(NEWSCREEN, s.ws_col, s.ws_row);
	(void)id;
}

int		a_print(int c)
{
	write(get_fd(0), &c, 1);
	return (1);
}

void	a_tputs(char *s)
{
	tputs(tgetstr(s, NULL), 1, a_print);
}

void	a_mv(int x, int y)
{
	tputs(tgoto(tgetstr("cm", NULL), x, y), 1, a_print);
}

void	a_handle_signal(void)
{
	int i;

	i = 0;
	while (i < 31)
	{
		if (i == SIGWINCH)
			signal(SIGWINCH, a_resize);
		else if (i == SIGTSTP || i == SIGCONT)
			signal(i, a_stopit);
		else
			signal(i, a_leave);
		i++;
	}
}
