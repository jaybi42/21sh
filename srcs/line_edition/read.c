/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 16:07:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/18 22:10:08 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <unistd.h>
#include "edit_line.h"

extern t_prompt g_prompt;

void		ft_ctrl_l(t_line *l)
{
	do_term("cl");
	if (l->size == 0)
		l->final_count = 0;
	print_prompt(g_prompt.rand, g_prompt.g_env, g_lenv, g_prompt.l);
	l->sizeprompt = l->sizeprompt + 3;
	print(l);
}

void		ft_ctrl_d(t_line *l)
{
	if (l->size != 0)
		ft_real_backspace(l);
	else
	{
		ft_putchar('\n');
		reset_line();
		exit(0);
	}
}

static int	get_stdin(t_line *l)
{
	while (42)
	{
		ft_bzero(l->buffer, 6);
		if (read(0, l->buffer, 6) == -1)
		{
			l->str = NULL;
			return (1);
		}
		if (l->buffer[0] != 10)
			actions(l);
		if (l->buffer[0] != 10 && ft_isprint(l->buffer[0]) == 1 && l->size <= MAXSIZE)
			ft_print_key(l);
		if (l->buffer[0] == 10)
		{
			ft_putchar('\n');
			return (0);
		}
	}
}

t_av		**read_init(t_line *l, t_ftl_root *hist)
{
	t_av	**av;
	int		ret;

	g_line = l;
	ft_init_line(l, hist);
	if (get_stdin(l) == 1)
		return (NULL);
	ret = parse_line(l, hist);
	av = parse_commands(ret == 0 ? l->str : "");
	l->str = NULL;
	l->oldstr = NULL;
	return (av);
}
