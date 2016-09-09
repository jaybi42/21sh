/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 16:58:08 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/09 16:22:16 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
** Have to receive the post-! from the editing
** exemples:
** "!!" => cmd = "!"
** "!22" => cmd = "22"
*/

static char		*get_line(t_ftl_root *hist, int pos)
{
	t_ftl_node	*node;

	if (ABS(pos) > (int)hist->size || hist->next == NULL || pos == 0)
		return (0);
	if (pos < 0)
		pos = hist->size + pos + 1;
	node = (t_ftl_node *)hist->next;
	while (--pos > 0)
		node = node->next;
	ft_putendl_fd(((t_hist *)node)->str, 2);
	return (((t_hist *)node)->str);
}

char			*parse_history(char *cmd, t_ftl_root *hist)
{
	int		pos;

	if (*cmd == '!')
		pos = -1;
	else
		pos = ft_atoi(cmd);
	return (get_line(hist, pos));
}
