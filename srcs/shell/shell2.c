/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 17:44:24 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/06 17:47:01 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void		clear_output(t_output *o)
{
	o->len = 0;
	o->string = xmalloc(2);
	if (o->string == NULL)
	{
		exit(1);
	}
	o->string[0] = '\0';
}

void		shell_init(t_shells *s)
{
	(g_debug) ? ft_dprintf(2, "-- {red}EXECUTION{eoc} --\n") : 0;
	s->find = -1;
	s->a = -1;
	clear_output(&s->all);
	clear_output(&s->output);
	s->output.string[0] = '\0';
	s->output.ret_code = 0;
	s->stack = xmalloc(sizeof(t_executor*) * 2);
	clear_stack(&s->stack, &s->stack_index);
}

void		shell_print_debug(t_shells *s, int w, int v)
{
	w = 0;
	while ((s->stack)[w])
	{
		if ((s->stack)[w]->ex.type == 0)
			ft_dprintf(2, "%d ; call bin |%s|\n", w, (s->stack)[w]->ex.path);
		else if ((s->stack)[w]->ex.type == 1)
			ft_dprintf(2, "%d ; call builtin |%s|\n", w, (s->stack)[w]->av.cmd);
		v = 0;
		ft_dprintf(2, "[");
		while ((s->stack)[w]->av.argv[v])
		{
			ft_dprintf(2, "'%s'", (s->stack)[w]->av.argv[v]);
			if ((s->stack)[w]->av.argv[v + 1])
				ft_dprintf(2, ", ");
			v++;
		}
		ft_dprintf(2, "]\n");
		w++;
	}
	(g_debug) ? ft_dprintf(2, "{green}----------------------{eoc}\n") : 0;
}

int			shell_pre_exec_logical_and_or(t_shells *s, t_av **av)
{
	if (av[(s->a)]->type == TYPE_OR || av[(s->a)]->type == TYPE_AND)
	{
		if ((s->find) == 0)
		{
			if (((s->output).ret_code == 0 && av[(s->a)]->type == TYPE_OR) ||
					((s->output).ret_code != 0 && av[(s->a)]->type == TYPE_AND))
			{
				(s->find) = 1;
				return (FALSE);
			}
		}
		else
			return (FALSE);
	}
	if (av[(s->a) + 1] != NULL && (av[(s->a) + 1]->type == TYPE_OR ||
				av[(s->a) + 1]->type == TYPE_AND) && ((s->find) == -1))
		(s->find) = 0;
	else
		(s->find) = -1;
	return (TRUE);
}

int			shell_pre_exec_logical_pipe(t_shells *s, t_av **av)
{
	if ((s->ex).type == -1)
	{
		set_retcode(127);
		return (FALSE);
	}
	if (av[(s->a) + 1] != NULL && av[(s->a) + 1]->type == TYPE_PIPE)
	{
		extend_stack(&(s->stack), &(s->stack_index), &(s->ex), av[(s->a)]);
		return (FALSE);
	}
	else
		update_stack(&(s->stack), &(s->stack_index), &(s->ex), av[(s->a)]);
	if (av[(s->a)]->type == TYPE_PIPE && (s->output).ret_code != 0)
		return (FALSE);
	return (TRUE);
}
