/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:01:52 by jguthert          #+#    #+#             */
/*   Updated: 2017/01/24 21:27:53 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "autocomp.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int			shell_pre_exec(t_shells *s, t_av **av)
{
	g_tete = av[(s->a)]->type;
	if (av[(s->a)]->cmd == NULL && (av[(s->a)]->type == TYPE_OR ||
		av[(s->a)]->type == TYPE_AND || av[(s->a)]->type == TYPE_PIPE ||
		(av[(s->a) + 1] != NULL && (av[(s->a) + 1]->type == TYPE_AND ||
		av[(s->a) + 1]->type == TYPE_OR || av[(s->a) + 1]->type == TYPE_PIPE))))
	{
		ft_dprintf(2, "%s: Invalid null command.\n", NAME);
		clear_output(&(s->output));
		(s->output).ret_code = 127;
	}
	if (av[(s->a)]->argv[0] == NULL)
	{
		clear_stack(&(s->stack), &(s->stack_index));
		return (FALSE);
	}
	(s->ex) = make_exec(av[(s->a)], &g_env, &g_lenv);
	if (!shell_pre_exec_logical_and_or(s, av))
		return (FALSE);
	if (!shell_pre_exec_logical_pipe(s, av))
		return (FALSE);
	return (TRUE);
}

int			shell_post_exec(t_shells *s, int ret)
{
	clear_stack(&(s->stack), &(s->stack_index));
	set_retcode((unsigned char)(s->output).ret_code);
	if (ret)
		x_strjoins(&(s->all).string, (size_t *)&(s->all).len,
				(s->output).string, (s->output).len);
	(s->all).ret_code = (s->output).ret_code;
	return (TRUE);
}

t_output	shell(t_av **av, int ret)
{
	t_shells s;

	shell_init(&s);
	while (av[++(s.a)] != NULL)
	{
		if (!shell_pre_exec(&s, av))
			continue ;
		(g_debug) ? shell_print_debug(&s, 0, 0) : 0;
		a_stop(0);
		s.output = do_exec(s.stack, ret);
		g_toto = 1;
		g_titi = 0;
		g_tata = 0;
		if (a_init() == -1)
		{
			ft_printf("error while getting the set\n");
			exit(127);
		}
		shell_post_exec(&s, ret);
	}
	return (s.all);
}

void		shell_exec_in(char *expr)
{
	t_av		**av;
	t_output	o;

	av = parse_commands(expr);
	o = shell(av, 0);
}

t_output	shell_exec(char *expr)
{
	t_av		**av;
	t_output	o;

	av = parse_commands(expr);
	o = shell(av, 1);
	return (o);
}
