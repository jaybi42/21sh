/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_screen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 23:59:09 by agadhgad          #+#    #+#             */
/*   Updated: 2016/04/01 00:07:01 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "acurse.h"

int		a_storage_data(int id, int tx, int ty)
{
	static int x;
	static int y;
	static int sig;

	if (id == NEWSIG)
		sig = x;
	else if (id == NEWSCREEN)
	{
		x = tx;
		y = ty;
	}
	else if (id == GETX)
		return (x);
	else if (id == GETY)
		return (y);
	return (0);
}

void	a_getscreen(int *x, int *y)
{
	*x = a_storage_data(GETX, 0, 0);
	*y = a_storage_data(GETY, 0, 0);
}

int		a_getscreen_y(void)
{
	return (a_storage_data(GETY, 0, 0));
}

int		a_getscreen_x(void)
{
	return (a_storage_data(GETX, 0, 0));
}

void	a_getcursor(int *x, int *y)
{
	*x = a_storage_data(GETCX, 0, 0);
	*y = a_storage_data(GETCY, 0, 0);
}
