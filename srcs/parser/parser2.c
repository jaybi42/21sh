/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 20:09:14 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/11 15:51:19 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			parse_handle_space(t_norm_parse *p, char *expr,
			int *t_ind, int *l_ind)
{
	if (!nparse_close(&(p->np), (p->i)))
		return (FALSE);
	while (expr[(p->i)] && is_whitespace(expr[(p->i)]) &&
			!is_intouchable((p->i), t_ind, l_ind))
		(p->i)++;
	if (!is_connector(expr, (p->i), t_ind, l_ind))
		nparse_extend(&(p->np), (p->i));
	(p->i)--;
	return (TRUE);
}

int			parse_handle_connector(t_norm_parse *p, char *expr,
			int *t_ind, int *l_ind)
{
	if (!nparse_close(&(p->np), (is_intouchable((p->i) - 1, t_ind, l_ind)
					|| !is_whitespace(expr[(p->i) - 1])) ? (p->i) : (p->i) - 1))
		return (FALSE);
	nparse_extend(&(p->np), (p->i));
	(p->np).type[(p->np).nb] = 2;
	if (!nparse_close(&(p->np), (p->i) + (p->i32_tmp) - 1))
		return (FALSE);
	if (is_intouchable((p->i) + (p->i32_tmp), t_ind, l_ind) ||
			!is_whitespace(expr[(p->i) + (p->i32_tmp)]))
		nparse_extend(&(p->np), (p->i) + (p->i32_tmp));
	(p->i) += (p->i32_tmp) - 1;
	return (TRUE);
}

void		parse_init(t_norm_parse *p, char *expr, int *t_ind, int *l_ind)
{
	nparse_init(&(p->np), expr);
	(p->i) = 0;
	while (expr[(p->i)] && is_whitespace(expr[(p->i)]) &&
			!is_intouchable((p->i), t_ind, l_ind))
		(p->i)++;
	nparse_extend(&(p->np), (p->i));
}

t_nparse	ret_parse(t_norm_parse *p, char *expr, int *t_ind, int *l_ind)
{
	if (!nparse_close(&(p->np), (p->i)))
		return ((p->np));
	(p->np).nb++;
	if ((p->i) > 0 && !is_intouchable((p->i) - 1, t_ind, l_ind) &&
		is_whitespace(expr[(p->i) - 1]))
		(p->np).nb -= 1;
	(p->np).failed = FALSE;
	return ((p->np));
}

t_nparse	parse(char *expr, int *t_ind, int *l_ind)
{
	t_norm_parse	p;

	parse_init(&p, expr, t_ind, l_ind);
	while (expr[(p.i)] != '\0')
	{
		if (is_intouchable((p.i), t_ind, l_ind))
		{
			while (expr[(p.i)] && is_intouchable((p.i), t_ind, l_ind))
				(p.i)++;
			(p.i)--;
		}
		else if (is_whitespace(expr[(p.i)]))
		{
			if (!parse_handle_space(&p, expr, t_ind, l_ind))
				return ((p.np));
		}
		else if (((p.i32_tmp) = is_connector(expr, (p.i), t_ind, l_ind)))
		{
			if (!parse_handle_connector(&p, expr, t_ind, l_ind))
				return ((p.np));
		}
		(p.i)++;
	}
	return (ret_parse(&p, expr, t_ind, l_ind));
}
