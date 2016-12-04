/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/25 15:47:39 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/04 20:39:23 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static void	parse_newline(char *str, t_ftl_root *hist)
{
	char	**ans;
	int		i;

	i = 0;
	ans = x_strsplit(str, '\n');
	while (ans[i])
		add_history(ans[i++], hist);
}

int			parse_line(t_line *l, t_ftl_root *hist)
{
	if (l->str == NULL)
		return (0);
	if (parse_history(l, hist) == 1)
		return (1);
	if (parse_quote(l) == 1)
		return (1);
	parse_newline(l->str, hist);
	return (0);
}
