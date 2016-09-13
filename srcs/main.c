/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/13 15:36:17 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/13 15:34:31 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>
#include <time.h>

static void		clean_exit(t_list *av, t_ftl_root *hist, t_line *l)
{
	int		nbr;

	(void)l; //free_line to do
	nbr = 0;
	if (av != NULL && *((t_av *)av->content)->arg != NULL)
		nbr = ft_atoi(*((t_av *)av->content)->arg);
	ft_lstdel(&av, free_av);
	put_history(hist);
	exit(nbr);
}


static int		sh21(t_list **g_env, t_list **l_env)
{
	t_list		*av;
	t_line		l;
	uint64_t	nbr;
	t_ftl_root	hist;

	ft_bzero(&l, sizeof(t_line));
	srand(time(NULL));
	nbr = rand();
	get_history(&hist);
	av = NULL;
	while (1)
	{
		catch_signal((t_prompt){nbr, *g_env, *l_env, 0, &l});
		print_prompt(nbr, *g_env, *l_env, &l);
		if (read_init(&av, &l, &hist) == 1)
			clean_exit(av, &hist, &l);
		if (shell(av, g_env, l_env, &hist) == 1)
			clean_exit(av, &hist, &l);
	}
	return (1);
}

int				main(void)
{
	t_list		*l_env;
	t_list		*g_env;

	if (init_env(&g_env, &l_env) == 1)
		return (1);
	if (init_term() == -1)
		return (1);
	if (sh21(&g_env, &l_env) == 1)
		return (1);
	return (0);
}
