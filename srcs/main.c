/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/13 15:36:17 by jguthert          #+#    #+#             */
/*   Updated: 2016/08/30 15:12:02 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>
#include <time.h>

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
	while (1)
	{
		catch_signal((t_prompt){nbr, *g_env, *l_env, 0, &l});
		print_prompt(nbr, *g_env, *l_env, &l);
		if (read_init(&av, &l, &hist) == 1)
			exit(0);
		if (shell(av, g_env, l_env) == 1)
			return (1);
		ft_lstdel(&av, free_av);
	}
	put_history(&hist);
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
