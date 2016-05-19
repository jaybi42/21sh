/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 19:08:23 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/10 18:47:37 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void		free_av(void *content, size_t size)
{
	t_av	*av;
	int		i;

	i = 0;
	(void)size;
	av = (t_av *)content;
	if (av != NULL)
	{
		if (av->all != NULL)
			ft_tabdel(av->all);
		if (av->arg != NULL)
			ft_strdel(av->arg);
		free(content);
		content = NULL;
	}
}

void		free_env(void *content, size_t size)
{
	(void)size;
	if (((t_env *)content) != NULL)
	{
		if (((t_env *)content)->name != NULL)
			ft_strdel(&((t_env *)content)->name);
		if (((t_env *)content)->value != NULL)
			ft_strdel(&((t_env *)content)->value);
		if (((t_env *)content)->str != NULL)
			ft_strdel(&((t_env *)content)->str);
		free(content);
		content = NULL;
	}
}
