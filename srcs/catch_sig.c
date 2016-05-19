/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   catch_sig.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/11 12:59:02 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/19 18:00:22 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <signal.h>

static t_prompt	g_prompt = {0, NULL, NULL};

static void		is_sig(int signum)
{
	ft_putchar('\n');
	ft_putstr("signal: ");
	ft_putnbr(signum);
	ft_putendl(" received.");
	print_prompt(g_prompt.rand, g_prompt.g_env, g_prompt.l_env);
}

void			catch_sig(int rand, t_list *g_env, t_list *l_env)
{
	g_prompt = (t_prompt){rand, g_env, l_env};
	signal(SIGINT, is_sig);
	signal(SIGTSTP, is_sig);
	signal(SIGQUIT, is_sig);
}
