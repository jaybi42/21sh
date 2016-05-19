/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   catch_sig.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/11 12:59:02 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/19 14:58:01 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <stdlib.h>

static t_sig	g_sig = {NULL};

static void		signal_cont(void)
{
	init_term();
	signal(SIGTSTP, is_sig);
	print_select(g_sig.root);
}

static void		signal_tstp(void)
{
	TERM		term;
	char		sig[2];

	do_termcap("cl");
	do_termcap("ve");
	tcgetattr(0, &term);
	tcsetattr(0, 0, &term);
	sig[0] = term.c_cc[VSUSP];
	sig[1] = 0;
	ioctl(0, TIOCSTI, sig);
	signal(SIGTSTP, SIG_DFL);
}

void			is_sig(int signum)
{
	if (signum == SIGWINCH)
		print_select(g_sig.root);
	else if (signum == SIGTSTP)
		signal_tstp();
	else if (signum == SIGCONT)
		signal_cont();
	else
	{
		reset_term();
		exit(signum);
	}
}

void			catch_sig(t_ftl_root *root)
{
	int		i;

	i = 1;
	g_sig.root = root;
	while (i < 32)
	{
		signal(i, is_sig);
		i++;
	}
}
