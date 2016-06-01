/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 16:07:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/31 15:01:39 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <curses.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <term.h>
#include <stdio.h>
#include <stdlib.h>

static void	fill_av(t_av *av, char *str)
{
	int		k;

	ft_bzero(av, sizeof(t_av));
	av->all = ft_strsplit_blank(str);
	k = 0;
	if (av->all != NULL)
	{
		av->cmd = av->all[0];
		av->arg = av->all + 1;
		while (av->arg[k] != NULL)
			k++;
	}
	av->argc = k;
}

static int	split_line(t_list **av_list, char *line)
{
	t_list	*temp;
	t_av	av;
	char	**tab2;
	int		i;

	i = 0;
	tab2 = ft_strsplit(line, ';');
	if (tab2 == NULL)
		return (1);
	while (tab2[i] != NULL)
	{
		fill_av(&av, tab2[i]);
		temp = ft_lstnew((void *)&av, sizeof(t_av));
		if (temp == NULL)
			return (1);
		ft_lstadd_last(av_list, temp);
		i++;
	}
	return (0);
}

int			read_init(t_list **av_list, t_line *l)
{
	ft_init_read(l, av_list);
	while (1)
	{
		ft_bzero(l->buffer, 6);
		read(0, l->buffer, 6);
		if (ENTER)
		{
			ft_putchar('\n');
			break ;
		}
		else if (LEFT)
			ft_left(l);
		else if (RIGHT)
			ft_right(l);
		else if (BACKSPACE)
			ft_backspace(l);
		else if (CTRL_R)
			ft_ctrl_r(l);
		else if (CTRL_L)
			ft_ctrl_l(l);
		else if (HOME)
			ft_home(l);
		else if (END)
			ft_end(l);
		else if (CUT)
			ft_cut(l);
		else if (PASTE)
			ft_paste(l);
		else if (CTRL_UP)
			ft_ctrl_up(l);
		else if (CTRL_DOWN)
			ft_ctrl_down(l);
		else
			ft_print_key(l);
		do_term("sc");
		do_goto("cm", 0, 15);
		do_term("cd");
		printf("count = %d, largeur = %d, size = %d, sizeprompt = %d\n STR = %s\n", l->count, l->largeur, l->size, l->sizeprompt, l->str);
		do_term("rc");
	}
	return (split_line(av_list, l->str));
}
