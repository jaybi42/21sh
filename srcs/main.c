/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/13 15:36:17 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/23 18:02:51 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>
#include <time.h>

t_list		*l_env;
t_list		*g_env;
t_ftl_root g_hist;

int				ret_exit(int state, int value)
{
	static int data = 0;

	if (state == SET)
	{
		data = value;
	}
	return (data);
}

void		clean_exit(int ret)
{
	put_history(&g_hist);
	a_stop(0);
	exit(ret);
}

void			ft_printmem(t_av **av)
{
	(void)av;
}

static int		sh21(void)
{
	t_av **av;
	t_line		l;
	uint64_t	nbr;
	t_output o;
	ft_bzero(&l, sizeof(t_line));
	srand(time(NULL));
	nbr = rand();
	get_history(&g_hist);
	av = NULL;
	while (1)
	{
		catch_signal((t_prompt){nbr, g_env, l_env, 0, &l});
		print_prompt(nbr, g_env, l_env, &l);
		if ((av = read_init(&l, &g_hist)) == NULL)
		{
			printf("special error\n");
			clean_exit(53);
		}
		o = shell(av, 0);
		xmasterfree();
	}
	return (1);
}

int				main(void)
{
	g_env = NULL;
	l_env = NULL;
	if (init_env(&g_env, &l_env) == 1)
		return (1);
	if (a_init() == -1)
		return (1);
	if (sh21() == 1)
		return (1);
	return (0);
}
