/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 19:08:23 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/28 20:50:28 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>

void		free_key(void *content, size_t size)
{
	char	*to_free;

	(void)size;
	to_free = (char *)content;
	if (to_free != NULL)
		ft_strdel(&to_free);
}

void		free_av(void *content, size_t size)
{
	t_av	*av;
	int		i;

	i = 0;
	(void)size;
	av = (t_av *)content;
	if (av != NULL)
	{
		if (av->argv != NULL)
			ft_tabdel(av->argv);
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
