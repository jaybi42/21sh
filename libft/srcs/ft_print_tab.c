/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_tab.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 17:30:13 by jguthert          #+#    #+#             */
/*   Updated: 2016/04/30 12:18:52 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_tab(char **tab, char *name)
{
	int	i;

	i = 0;
	ft_putstr("----- ");
	ft_putstr(name);
	ft_putendl(" -----");
	while (tab[i] != NULL)
	{
		ft_putnbr(i);
		ft_putstr(" : [");
		ft_putstr(tab[i]);
		ft_putstr("]\n");
		i++;
	}
	ft_putendl("---------------");
}
