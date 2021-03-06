/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   catch_sig.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/11 12:59:02 by jguthert          #+#    #+#             */
/*   Updated: 2017/01/18 17:12:03 by malaine          ###   ########.fr       */
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

	if ((g_quithist = 0) == 0 && signum == SIGCHLD)
		return ;
	else if (signum > 3 && signum < 28)
		signal(signum, SIG_IGN);
	else if (signum == SIGWINCH && g_prompt.l != NULL)
	{
		ioctl(0, TIOCGWINSZ, &w);
		g_prompt.l->largeur = w.ws_col;
		g_prompt.l->hauteur = w.ws_row;
		g_line->resize = 1;
		ft_ctrl_l(g_line);
	}
	else if (g_prompt.son == 0 && g_inheredoc_2 != 1)
	{
		if (g_heredoc != NULL)
			ft_init_line_heredoc(g_heredoc);
		if (g_line != NULL)
			ft_end(g_line);
		ft_putstr("\n");
		(g_prompt.onshell) ?
		print_prompt(g_prompt.rand, g_prompt.g_env, g_lenv, g_prompt.l) : 0;
		if (g_line != NULL && (g_line->quit = -1) == -1)
			reset_line();
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
