/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:01:52 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/23 16:51:04 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include "autocomp.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

/*
** SHELL_PRE_EXEC HANDLE THE LOGICAL EXECUTION OF THE COMMAND,
** IT ALSO HANDLE THE open() of the redirection
*/
int shell_pre_exec(t_shells *s, t_av **av)
{
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
	(s->ex) = make_exec(av[(s->a)], &g_env, &l_env);
	if (!shell_pre_exec_logical_and_or(s, av))
		return (FALSE);
	if (!shell_pre_exec_logical_pipe(s, av))
		return (FALSE);
	return (TRUE);
}

/*
**  SHELL_POST_EXEC handle the cleaning of the executed command
** 	+ set the ret_code ($?)
*/

int shell_post_exec(t_shells *s, int ret)
{
	clear_stack(&(s->stack), &(s->stack_index));
	set_retcode((unsigned char)(s->output).ret_code);
	if (ret)
		x_strjoins(&(s->all).string,(size_t *)&(s->all).len,(s->output).string,
			(s->output).len);
	(s->all).ret_code = (s->output).ret_code;
	return (TRUE);
}

/*
 HOW DOES THIS WORK?

 this is a simple system of stack.

 the stack is extended only when the next command is a pipe.
 It's for handle the async mode.

 for: "ls | cat -e"

 we will have a stack of this sort

 1 | cat -e  |
 0 | ls      |

for: "echo ok ; ls "

 0 | echo ok |
then
 0 | ls      |

etc..
*/
t_output		shell(t_av **av, int ret)
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
		if (a_init() == -1)
		{
			ft_printf("error while getting the set\n");
			exit(127);
		}
		shell_post_exec(&s, ret);
	}
	return (s.all);
}

/*
 ** give him an expr (example: "echo ok | cat -e")
 ** give you the return output and the last ret_code
*/
t_output shell_exec(char *expr)
{
	t_av **av;
	t_output o;
	av = parse_commands(expr);
	o = shell(av, 1);
	return (o);
}
