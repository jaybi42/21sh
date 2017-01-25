/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/09 16:33:51 by jguthert          #+#    #+#             */
/*   Updated: 2017/01/09 16:34:05 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <shell.h>

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

static int	find_and_replace(t_list *link, t_av av, t_list **g_env)
{
	while (link != NULL)
	{
		if (ft_strcmp(((t_env *)link->content)->name, *av.arg) == 0)
		{
			unset_link(g_env, link);
			return (0);
		}
		link = link->next;
	}
	return (-1);
}

int			bi_unset(t_av av, t_list **g_env, t_list **g_lenv)
{
	t_list	*link;

	(void)g_env;
	if (g_env == NULL || *av.arg == NULL)
		return (1);
	link = *g_lenv;
	if (av.argc > 1)
		return (print_error(av, 0));
	else if (av.argc == 1 && ft_isalpha(*av.arg[0]) == 0)
		return (print_error(av, 5));
	else if (av.argc == 1 && ft_strisprint(*av.arg) == 0)
		return (print_error(av, 4));
	return (find_and_replace(link, av, g_lenv));
}
