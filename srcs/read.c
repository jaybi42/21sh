/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 16:07:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/06 15:52:24 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>

/*
static void	fill_av(t_av *av, char *str)
{
	int		k;

	ft_bzero(av, sizeof(t_av));
	av->argv = ft_strsplit_blank(str);
	k = 0;
	if (av->argv != NULL)
	{
		av->cmd = av->argv[0];
		av->arg = av->argv + 1;
		while (av->arg[k] != NULL)
			k++;
	}
	av->argc = k;
}
*/
/*
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
*/

#include <stdio.h> //

void     debug_editline(t_line *l)
{
	do_term("sc");
	do_goto("DO", 0, 5);
	do_term("cd");
	do_goto("DO", 0, 5);
	printf("count = %d, largeur = %d, size = %d, sizeprompt = %d\n STR = %s\n", l->count,\
		   l->largeur, l->size, l->sizeprompt, l->str);
	do_term("rc");
}


t_av	**read_init(t_line *l, t_ftl_root *hist)
{
	g_line = l;
	ft_init_line(l, hist);
	while (1)
	{
		ft_bzero(l->buffer, 6);
		if (read(0, l->buffer, 6) == -1)
			return (NULL);
		if (l->buffer[0] != 10)
			actions(l);
		if (l->buffer[0] != 10 && ft_isprint(l->buffer[0]) == 1)
			ft_print_key(l);
//		debug_editline(l);
		if (l->buffer[0] == 10)
		{
			ft_putchar('\n');
			break ;
		}
	}
	if (parse_line(l, hist) == 0)
		return (parse_commands(l->str));
	return (parse_commands(""));
}
