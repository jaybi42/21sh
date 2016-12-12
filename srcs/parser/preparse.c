/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 23:02:52 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/11 16:37:48 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		handle_tilde_init(t_norm_handle_tilde *t, char *s, char **env)
{
	(t->tmp) = NULL;
	if (!((t->tmpx) = fstrsplit(s, ft_strlen(s), char_is_whitespace)))
		return (FALSE);
	if (!((t->home) = find_home(env)))
		return (FALSE);
	(t->i) = -1;
	while ((t->tmpx)[++(t->i)])
		;
	(t->ret) = xmalloc(sizeof(char *) * ((t->i) + 2));
	(t->i) = 0;
	(t->find) = 0;
	return (TRUE);
}

int		handle_tilde_end(t_norm_handle_tilde *t)
{
	(t->ret)[(t->i)] = NULL;
	if (!(t->find))
		return (FALSE);
	(t->ret_s) = NULL;
	(t->len) = 0;
	(t->i) = -1;
	while ((t->ret)[++(t->i)])
		(t->len) += ft_strlen((t->ret)[(t->i)]) + 1;
	if (!((t->ret_s) = xmalloc(sizeof(char) * ((t->len) + 1))))
		return (FALSE);
	(t->i) = -1;
	(t->len) = 0;
	(t->ret_s)[0] = '\0';
	while ((t->ret)[++(t->i)])
	{
		x_strjoins(&(t->ret_s), &(t->len), (t->ret)[(t->i)],
		ft_strlen((t->ret)[(t->i)]));
		x_strjoins(&(t->ret_s), &(t->len), " ", 1);
	}
	return (TRUE);
}

char	*handle_tilde(char *s, char **env)
{
	t_norm_handle_tilde t;

	if (!handle_tilde_init(&t, s, env))
		return (s);
	while ((t.tmpx)[(t.i)])
	{
		(t.tmp) = tilde_path((t.tmpx)[(t.i)], (t.home));
		if ((t.tmp) != NULL)
		{
			(t.find) = 1;
			(t.ret)[(t.i)] = (t.tmp);
		}
		else
			(t.ret)[(t.i)] = (t.tmpx)[(t.i)];
		(t.i)++;
	}
	if (!handle_tilde_end(&t))
		return (s);
	return ((t.ret_s));
}

void	decortique_parse_while(t_norm_d_p *t, char *expr)
{
	if ((t->p)->type[(t->i)] == 2)
	{
		(t->o) = shell_exec(cpy_a_to_b(expr, (t->p)->begin[(t->i)],
		(t->p)->end[(t->i)]));
		(t->ts)[(t->i)] = apply_var(defragement((t->o).string), FALSE);
	}
	else if ((t->p)->type[(t->i)] == 1)
		(t->ts)[(t->i)] = cpy_a_to_b(expr, (t->p)->begin[(t->i)],
		(t->p)->end[(t->i)]);
	else
		(t->ts)[(t->i)] = apply_var(defragement(cpy_a_to_b(expr,
			(t->p)->begin[(t->i)], (t->p)->end[(t->i)])), TRUE);
	if ((t->p)->type[(t->i)] == 0 || (t->p)->type[(t->i)] == 1)
		(t->marked_ind)[(t->i)] = 1;
	else
		(t->marked_ind)[(t->i)] = 0;
	(t->i)++;
}

char	*decortique_parse(char *expr, size_t l, int **t_ind, int **l_ind)
{
	t_norm_d_p t;

	(t.marked_ind) = xmalloc(sizeof(int) * (l + 1));
	(t.ts) = xmalloc(sizeof(char*) * (l + 1));
	(t.ts)[0] = NULL;
	(t.p) = parse_it2(expr, l, (t_delimiter *)&g_delimiter_quo, 3);
	(t.i) = 0;
	while ((t.i) < (t.p)->nb)
	{
		decortique_parse_while(&t, expr);
	}
	(t.ts)[(t.i)] = NULL;
	return (join_string_array((t.ts), (t.marked_ind), t_ind, l_ind));
}
