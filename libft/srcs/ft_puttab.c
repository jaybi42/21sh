/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puttab.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 13:34:36 by agadhgad          #+#    #+#             */
/*   Updated: 2016/03/29 04:47:00 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		ft_affichage(int i, const char *s)
{
	ft_putnbr(i);
	ft_putstr(": ");
	ft_putstr_dbcote(s);
	ft_putchar('\n');
}

void			ft_puttab(char **tabs)
{
	int i;

	i = 0;
	while (tabs[i] != 0)
	{
		ft_affichage(i, tabs[i]);
		i++;
	}
	ft_affichage(i, "NULL");
}
