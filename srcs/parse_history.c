/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 16:58:08 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/06 17:44:07 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

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
	return (((t_hist *)node)->str);
}

static int		new_cmd(t_av av, t_ftl_root *hist, int pos)
{
	char	*str;

	str = get_line(hist, pos);
	ft_putendl_fd(str, 2);
	(void)av;
/*	if (*av.arg != NULL)
		str1 = ft_strjoin(str, av.arg);
	do_shell_again_at_parsing(str1);
*/
	return (0);
}

int			parse_history(t_av av, t_ftl_root *hist)
{
	int		pos;

	if (ft_strncmp("!", av.cmd, 1) != 0)
		return (0);
	av.cmd++;
	if (*av.cmd == '!')
		pos = -1;
	else
		pos = ft_atoi(av.cmd);
	return (new_cmd(av, hist, pos));
}
