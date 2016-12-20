/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   catch_sig.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/11 12:59:02 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/20 18:46:55 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <signal.h>

static void				sig_action(void *action)
{
	struct sigaction	sig;

	sig.sa_flags = 0;
	sig.sa_handler = action;
	sigaction(SIGINT, &sig, 0);
}

static void				is_sig(int signum)
{
	struct winsize		w;

	if (signum > 3 && signum < 28)
	{
		if (signum == SIGCHLD)
			return;
		signal(signum, SIG_IGN);
	}
	else if (signum == SIGWINCH && g_prompt.l != NULL)
	{
		ioctl(0, TIOCGWINSZ, &w);
		g_prompt.l->largeur = w.ws_col;
		g_prompt.l->hauteur = w.ws_row;
	}
	else if (g_prompt.son == 0)
	{
		ft_putstr("\n");
		(g_prompt.onshell) ?
		print_prompt(g_prompt.rand, g_prompt.g_env, g_lenv, g_prompt.l) : 0;
		if (g_line != NULL)
		{
			reset_line();
			g_line->quit = -1;
		}
	}
	g_prompt.son = 0;
}

void					catch_signal(void)
{
	int i;

	i = 0;
	while (i++ < 28)
		signal(i, is_sig);
}

void					sig_on(void)
{
	sig_action(SIG_DFL);
}

void					sig_off(void)
{
	sig_action(SIG_IGN);
}
