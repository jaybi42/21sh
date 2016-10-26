/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_clear.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/01 00:04:39 by agadhgad          #+#    #+#             */
/*   Updated: 2016/04/01 00:04:40 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "acurse.h"

void	a_init_clear(void)
{
	char *s;

	s = tgetstr("cl", &s);
	ft_putstr_fd(s, get_fd(0));
}

void	a_clear(void)
{
	static char *s;

	a_mv(0, 0);
	if (!s)
		s = tgetstr("cd", &s);
	ft_putstr_fd(s, get_fd(0));
}
