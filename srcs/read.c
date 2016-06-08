/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 16:07:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/06/08 15:37:16 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>

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
	char	**tab_av;
	int		i;

	i = 0;
	tab_av = ft_strsplit(line, ';');
	if (tab_av == NULL)
		return (1);
	while (tab_av[i] != NULL)
	{
		fill_av(&av, tab_av[i]);
		temp = ft_lstnew((void *)&av, sizeof(t_av));
		if (temp == NULL)
			return (1);
		ft_lstadd_last(av_list, temp);
		i++;
	}
	return (0);
}

#include <stdio.h> //

static void     debug_editline(t_line *l)
{
	do_term("sc");
	do_term("cd");
	do_goto("DO", 0, 10);
	printf("count = %d, largeur = %d, size = %d, sizeprompt = %d\n STR = %s\n", l->count,\
		   l->largeur, l->size, l->sizeprompt, l->str);
	do_term("rc");
}

int			read_init(t_list **av_list, t_line *l)
{
	*av_list = NULL;
	ft_init_line(l);
	while (1)
	{
		ft_bzero(l->buffer, 6);
		if (read(0, l->buffer, 6) == -1)
			return (1);
		if (l->buffer[0] == 10)
		{
			ft_putchar('\n');
			break ;
		}
		else if (actions(l) == 1)
			ft_print_key(l);
		debug_editline(l);
	}
	return (split_line(av_list, l->str));
}
