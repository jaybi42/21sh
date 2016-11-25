/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unsetenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/20 13:29:49 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/19 17:58:47 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static void	unset_link(t_list **g_env, t_list *link)
{
	t_list	*prev;

	prev = *g_env;
	if (*g_env == link)
	{
		*g_env = prev->next;
		ft_lstdelone(&prev, free_env);
		return ;
	}
	while (prev->next != link)
		prev = prev->next;
	prev->next = link->next;
	ft_lstdelone(&link, free_env);
}

int			bi_unsetenv(t_av av, t_list **g_env, t_list **l_env)
{
	t_list	*link;

	(void)l_env;
	if (g_env == NULL || *av.arg == NULL)
		return (1);
	link = *g_env;
	if (av.argc > 1)
		return (print_error(av, 0));
	else if (av.argc == 1 && ft_isalpha(*av.arg[0]) == 0)
		return (print_error(av, 5));
	else if (av.argc == 1 && ft_strisalnum(*av.arg) == 0)
		return (print_error(av, 4));
	while (link != NULL)
	{
		if (ft_strstr(((t_env *)link->content)->str, *av.arg) != NULL)
		{
			unset_link(g_env, link);
			return (0);
		}
		link = link->next;
	}
	return (-1);
}
