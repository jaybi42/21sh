/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gbrace.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 16:00:59 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/06 18:56:22 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void	insert_in_arr(t_brak *b, char **t_add)
{
	int		i;
	int		a;
	char	**tmp;
	int		x;

	x = 0;
	tmp = xmalloc(sizeof(char *) * b->len);
	a = -1;
	while (++a < b->ta)
	{
		i = -1;
		while (t_add[++i])
			tmp[x++] = x_strjoin2(b->t[a], b->i_a[a],
			t_add[i], ft_strlen(t_add[i]));
	}
	tmp[x] = NULL;
	b->t = tmp;
	a = -1;
	while (b->t[++a] != NULL)
		b->i_a[a] = ft_strlen(b->t[a]);
	b->ta = a;
}

int		play_with_bracket(t_brak *b, char *s)
{
	char	**t_to_add;
	int		d;
	int		len;
	int		i;

	if ((t_to_add = ret_arr(s)) != NULL)
	{
		i = 0;
		insert_in_arr(b, t_to_add);
		d = 1;
		len = len_depth(s);
		return (len + 1);
	}
	else
	{
		update_t(&b->t, &b->i_a, &b->ta, '{');
	}
	return (1);
}

void	close_t(char ***p_t, int **p_i_a, int *p_ta)
{
	int i;

	i = 0;
	while (i < (*p_ta))
	{
		(*p_t)[i][(*p_i_a)[i]] = '\0';
		i++;
	}
	(*p_t)[i] = NULL;
}

char	**bracesplit(char *s)
{
	t_brak	b;
	int		i;

	b.len = ft_strlen(s) + 1;
	if (!(b.t = xmalloc(sizeof(char *) * b.len)) ||
			!(b.i_a = xmalloc(sizeof(int) * b.len)))
		return (NULL);
	if (!(b.t[0] = xmalloc(sizeof(char) * b.len)))
		return (NULL);
	b.i_a[0] = 0;
	b.ta = 1;
	i = 0;
	while (s[i])
	{
		if (s[i] == '{')
			i += play_with_bracket(&b, s + i + 1);
		else
		{
			update_t(&b.t, &b.i_a, &b.ta, s[i]);
			i++;
		}
	}
	close_t(&b.t, &b.i_a, &b.ta);
	return (b.t);
}

char	**brace_handler(char *s)
{
	char **exprs;

	exprs = bracesplit(s);
	return (exprs);
}
