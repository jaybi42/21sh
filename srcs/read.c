/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 16:07:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/19 17:59:20 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

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
	char	**tab;
	int		i;

	i = 0;
	tab = ft_strsplit(line, ';');
	if (tab == NULL)
		return (1);
	while (tab[i] != NULL)
	{
		fill_av(&av, tab[i]);
		temp = ft_lstnew((void *)&av, sizeof(t_av));
		if (temp == NULL)
			return (1);
		ft_lstadd_last(av_list, temp);
		i++;
	}
	return (0);
}

int			read_init(t_list **av_list)
{
	char	*line;

	*av_list = NULL;
	line = NULL;
	while (line == NULL)
	{
		if (get_next_line(0, &line) < 0)
			return (1);
	}
	return (split_line(av_list, line));
}
