/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftl_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 15:03:50 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/11 18:15:48 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ftl_init(t_ftl_root *l, size_t node_size)
{
	*l = (t_ftl_root){(t_ftl_node*)l, (t_ftl_node*)l, 0, node_size};
}

t_ftl_root	ftl_uninit(void)
{
	return ((t_ftl_root){NULL, NULL, 0, 0});
}
