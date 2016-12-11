/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparse3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 23:09:34 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/11 15:47:44 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int			handle_delimiter2_init(t_norm_parse2 *t, int *a)
{
	while ((*a) < (t->l))
	{
		if (ft_strncmp((t->expr) + (t->i), (t->d)[(*a)].name,
		ft_strlen((t->d)[(*a)].name)) == 0)
		{
			if ((t->p)->current != EMPTY &&
			(t->d)[(t->p)->current].wait_another
			== TRUE && (t->p)->current == (*a))
			{
				(t->p)->end[(t->p)->nb] = (t->i);
				reset_current((t->p), (t->i));
			}
			else if ((t->p)->current != EMPTY &&
			(t->d)[(t->p)->current].do_recursivity == TRUE &&
			(t->p)->current == (*a))
				update_new((t->p), (t->i), (*a));
			else if ((t->p)->current == EMPTY)
				update_new((t->p), (t->i), (*a));
			(t->i) += 1;
			return (FALSE);
		}
		(*a)++;
	}
	return (TRUE);
}

void		handle_delimiter2(t_norm_parse2 *t)
{
	int a;

	a = 0;
	if (!handle_delimiter2_init(t, &a))
		return ;
	if ((t->p)->current != EMPTY &&
		(t->d)[(t->p)->current].is_redirection == TRUE
		&& (t->p)->one_arg == FALSE
		&& char_is_whitespace((t->expr)[(t->i)]) == FALSE)
		(t->p)->one_arg = TRUE;
	else if ((t->p)->current != EMPTY &&
		(t->d)[(t->p)->current].is_redirection == TRUE && (t->p)->one_arg ==
		TRUE && char_is_whitespace((t->expr)[(t->i)]) == TRUE)
	{
		(t->p)->end[(t->p)->nb] = (t->i);
		reset_current((t->p), (t->i));
	}
	(t->i) += 1;
}

t_parse		*parse_it2(char *expr, int len, t_delimiter *d, int l)
{
	t_norm_parse2 t;

	t.expr = expr;
	t.len = len;
	t.d = d;
	t.l = l;
	if (!((t.p) = xmalloc(sizeof(t_parse))) ||
			!((t.p)->begin = xmalloc(sizeof(int) * ft_strlen((t.expr)))) ||
			!((t.p)->end = xmalloc(sizeof(int) * ft_strlen((t.expr)))) ||
			!((t.p)->type = xmalloc(sizeof(int) * ft_strlen((t.expr)))))
		exit(0);
	(t.i) = 0;
	(t.p)->nb = 0;
	(t.p)->current = -1;
	(t.p)->begin[0] = 0;
	(t.p)->type[0] = -1;
	while ((t.i) < (t.len) && (t.expr)[(t.i)] != '\0')
		handle_delimiter2(&t);
	if ((t.p)->current != EMPTY && (t.d)[(t.p)->current].wait_another == TRUE)
		ft_printf("[!] we were waiting another |%s|\
				adding one for u\n", (t.d)[(t.p)->current].name);
	(t.p)->end[(t.p)->nb] = (t.i);
	(t.p)->nb++;
	return ((t.p));
}
