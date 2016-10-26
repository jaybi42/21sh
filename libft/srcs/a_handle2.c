/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_handle2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 23:51:54 by agadhgad          #+#    #+#             */
/*   Updated: 2016/05/12 15:41:34 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "acurse.h"

void	a_stopit(int id)
{
	static struct termios	term;
	static char				cp[2] = {032, 0};
	int						fd;

	if (id == SIGTSTP)
	{
		if (!isatty(1))
			return ;
		tcgetattr(0, &term);
		tcsetattr(0, 0, &term);
		a_store_old_term(0);
		signal(SIGTSTP, SIG_DFL);
		ioctl(0, TIOCSTI, cp);
	}
	else if (id == SIGCONT)
	{
		fd = open(ttyname(0), O_WRONLY);
		tcsetattr(0, TCSANOW, &term);
		get_fd(fd);
		a_store_old_term(INIT);
		signal(SIGTSTP, a_stopit);
	}
}

void	a_leave(int id)
{
	a_exit(id);
}
