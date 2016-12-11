/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparse2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 22:55:53 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/11 15:44:03 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int		check_var_init(t_c_v *t, char *s)
{
	(t->tmpe) = handle_var(KV_GET, s, NULL);
	if ((t->tmpe) != NULL)
	{
		if (!((t->ret) = xmalloc(sizeof(char *) * 3)))
			return (-1);
		(t->ret)[0] = s;
		(t->ret)[1] = (t->tmpe);
		(t->ret)[2] = NULL;
		return (1);
	}
	(t->i) = 0;
	return (0);
}

char	**check_var(char *s, char **env)
{
	t_c_v t;

	if ((t.x = check_var_init(&t, s)) == -1)
		return (NULL);
	if (t.x == 1)
		return ((t.ret));
	while (env[(t.i)])
	{
		(t.tmp) = x_strsplit(env[(t.i)], '=');
		if ((t.tmp) != NULL && (t.tmp)[0] != NULL && (t.tmp)[1] != NULL)
			if (ft_strncmp(s, (t.tmp)[0], ft_strlen((t.tmp)[0])) == 0)
			{
				(t.ret) = xmalloc(sizeof(char *) * 3);
				if (!(t.ret))
					return (NULL);
				(t.ret)[0] = (t.tmp)[0];
				(t.ret)[1] = (t.tmp)[1];
				(t.ret)[2] = NULL;
				return ((t.ret));
			}
		(t.i)++;
	}
	return (NULL);
}

int		apply_var_init(t_apply_var *t, char **s, int do_extra)
{
	(t->env) = convert_env(g_env, g_lenv);
	if (do_extra == TRUE)
		(*s) = handle_tilde((*s), (t->env));
	(t->i) = -1;
	(t->len) = 0;
	while ((*s)[++(t->i)])
	{
		if ((*s)[(t->i)] == '$' && ((t->tmp) = check_var((*s) + (t->i) + 1,
			(t->env))) != NULL)
			(t->len) += ft_strlen((t->tmp)[1]);
		else
			(t->len) += 1;
	}
	if (!((t->ns) = xmalloc(sizeof(char) * ((t->len) + 1))))
		return (FALSE);
	(t->i) = -1;
	(t->len) = 0;
	return (TRUE);
}

char	*apply_var(char *s, int do_extra)
{
	t_apply_var t;

	if (!apply_var_init(&t, &s, do_extra))
		return (NULL);
	while (s[++(t.i)])
	{
		if (s[(t.i)] == '$' && ((t.tmp) = check_var(s + (t.i) + 1, (t.env)))
		!= NULL)
		{
			x_strjoins(&(t.ns), &(t.len), (t.tmp)[1], ft_strlen((t.tmp)[1]));
			(t.ns)[(t.len)] = '\0';
			(t.i) += ft_strlen((t.tmp)[0]);
		}
		else
		{
			(t.ns)[(t.len)++] = s[(t.i)];
			(t.ns)[(t.len)] = '\0';
		}
	}
	(t.ns)[(t.len)] = '\0';
	return ((t.ns));
}
