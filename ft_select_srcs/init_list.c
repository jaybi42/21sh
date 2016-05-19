/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/11 17:54:18 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/17 14:51:43 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int			init_list(t_ftl_root *root, char **av, int ac)
{
	t_elem	elem;

	ft_bzero(&elem, (sizeof(t_elem)));
	while (--ac > 1)
	{
		elem.name = av[ac];
		elem.name_len = ft_strlen(elem.name);
		elem.pos = ac - 1;
		if (ftl_push_front(root, (FTL_NODE *)(&elem)) != 0)
			return (1);
	}
	elem.name = av[ac];
	elem.cursor = 1;
	elem.pos = ac - 1;
	elem.name_len = ft_strlen(elem.name);
	if (ftl_push_front(root, (FTL_NODE *)(&elem)) != 0)
		return (1);
	return (0);
}
