/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gparse3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 16:55:40 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/05 16:56:48 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void		add_char(char **exprs, int *len, char c)
{
	int	succeed;
	int	tmp;

	succeed = 1;
	tmp = 0;
	while (tmp < (*len))
	{
		if ((*exprs)[tmp] == c)
		{
			succeed = 0;
			break ;
		}
		tmp++;
	}
	if (succeed)
	{
		(*exprs)[(*len)++] = c;
		(*exprs)[(*len)] = '\0';
	}
}

int			quarter_run(char *expr, int *tmp_len, t_norm_quarter *q)
{
	while (expr[(q->i)] && expr[(q->i)] != ']')
	{
		if (expr[(q->i)] == '-' && ((q->i) != 0 && (size_t)((q->i) + 1) !=
			ft_strlen(expr)))
		{
			if (expr[(q->i) - 1] <= expr[(q->i) + 1])
			{
				(q->c) = expr[(q->i) - 1] - 1;
				while (++(q->c) <= expr[(q->i) + 1])
					add_char(&(q->globbing)->exprs, &(q->a_str),
					expr[(q->i) - 1] - 1);
			}
			else
				add_char(&(q->globbing)->exprs, &(q->a_str), expr[(q->i)]);
		}
		else
			add_char(&(q->globbing)->exprs, &(q->a_str), expr[(q->i)]);
		(q->i)++;
	}
	if (expr[(q->i)] != ']' || (q->i) == 0)
		return (FALSE);
	(*tmp_len) = (q->i);
	return (TRUE);
}

t_globbing	*quarter(char *expr, int *tmp_len)
{
	t_norm_quarter q;

	if (!(q.globbing = xmalloc(sizeof(t_globbing))))
		return (NULL);
	if (!(q.globbing->exprs = xmalloc(sizeof(char) * 127)))
		return (NULL);
	q.globbing->list = 1;
	q.i = 0;
	q.a_str = 0;
	if (expr[q.i] == '^')
	{
		q.globbing->is_looking = 0;
		q.i++;
	}
	else
		q.globbing->is_looking = 1;
	if (!quarter_run(expr, tmp_len, &q))
		return (NULL);
	return (q.globbing);
}

t_globbing	*rec_g(char *expr, int *i, int iquare)
{
	t_globbing	*g;
	int			c;
	int			careone;

	if (!(g = xmalloc(sizeof(t_globbing))))
		return (NULL);
	c = 0;
	careone = 0;
	while (expr[(*i) + c] && expr[(*i) + c] != '?' && expr[(*i) + c] != '*')
	{
		if (iquare && expr[(*i) + c] == '[')
			break ;
		c++;
	}
	g->is_looking = 1;
	g->list = 0;
	g->exprs = cpy_a_to_b2(expr, (*i), (*i) + c);
	(*i) += c - 1;
	return (g);
}
