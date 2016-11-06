/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/25 15:47:39 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/06 15:35:31 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int			parse_line(t_line *l , t_ftl_root *hist)
{
	if (l->str == NULL)
		return (0);
	if (parse_history(l, hist) == 1)
		return (1);
	add_history(l->str, hist);
	if (parse_quote(l) == 1)
		return (1);
	return (0);
}
