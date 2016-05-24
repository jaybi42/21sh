/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   catch_sig.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/11 12:59:02 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/24 11:54:24 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <signal.h>

static t_prompt	g_prompt = {0, NULL, NULL, 0};

static void	sig_action(void *action)
{
	struct sigaction sig;

	sig.sa_flags = 0;
	sig.sa_handler = action;
	sigaction(SIGINT, &sig, 0);
}

static void	is_sig(int signum)
{
	ft_putchar('\n');
	ft_putstr("signal: ");
	ft_putnbr(signum);
	ft_putendl(" received.");
	if (g_prompt.son == 0)
		print_prompt(g_prompt.rand, g_prompt.g_env, g_prompt.l_env);
}

void	catch_signal(int rand, t_list *g_env, t_list *l_env, bool son)
{
	g_prompt = (t_prompt){rand, g_env, l_env, son};
	signal(SIGINT, is_sig);
	signal(SIGTSTP, is_sig);
	signal(SIGQUIT, is_sig);
}

void		sig_on(void)
{
	sig_action(SIG_DFL);
}

void		sig_off(void)
{
	sig_action(SIG_IGN);
}
