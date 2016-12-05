/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gparse2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 16:34:48 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/05 16:46:39 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int		ret_init(t_arr *r, char *s)
{
	(r->i) = 0;
	(r->len) = ft_strlen(s);
	if (!((r->t_s) = xmalloc(sizeof(char *) * ((r->len) + 1))))
		return (FALSE);
	if (!((r->t_a) = xmalloc(sizeof(int) * ((r->len) + 1))))
		return (FALSE);
	(r->t_i) = 0;
	(r->t_a)[(r->t_i)] = 0;
	if (!((r->t_s)[(r->t_i)++] = xmalloc(sizeof(char) * ((r->len) + 1))))
		return (FALSE);
	(r->t_s)[(r->t_i)] = NULL;
	(r->t_s)[(r->t_i) - 1][0] = '\0';
	(r->tog) = 1;
	return (TRUE);
}

int		ret_o_b_place(t_arr *r, char **a_t, int i_t)
{
	char	*old_tmp;

	if (!(old_tmp = x_strdup2((r->t_s)[(r->t_i) - 1], (r->t_a)[(r->t_i) - 1])))
		return (FALSE);
	i_t = 0;
	while (a_t[i_t] != NULL)
	{
	/*
	**	Un conseil fou tout ce qu'il y a dans ce while dans une fonction fille
	**	vu que t'es deja a 6 fonction sa t'en feras 7 et j'ai renommer ta variable i_tmp
	**	en i_t et ta variable a_tmp en a_t
	*/
		if ((r->t_s)[(r->t_i) - 1 + i_t] != NULL)
		{
			(r->t_s)[(r->t_i) - 1 + i_t] = x_strjoin2((r->t_s)[(r->t_i) - 1 + i_t],
			(r->t_a)[(r->t_i) - 1 + i_t], a_t[i_t],ft_strlen(a_t[i_t]));
			(r->t_a)[(r->t_i) - 1 + i_t] += ft_strlen(a_t[i_t]);
		}
		else
		{
			(r->t_s)[(r->t_i) - 1 + i_t] = x_strjoin2(old_tmp, ft_strlen(old_tmp),
					a_t[i_t], ft_strlen(a_t[i_t]));
			(r->t_a)[(r->t_i) - 1 + i_t] = ft_strlen(old_tmp) +
			ft_strlen(a_t[i_t]);
		}
		i_t++;
	}
	return (TRUE);
}

int		ret_open_bracket(t_arr *r, char *s)
{
	char	**a_tmp;
	int		i_tmp;
	int		xx;

	a_tmp = ret_arr(s + (r->i) + 1);
	if (a_tmp == NULL)
		return (FALSE);
	i_tmp = 0;
	ret_o_b_place(r, a_tmp, i_tmp);
	(r->t_i) += i_tmp - 1;
	(r->tog) += i_tmp - 1;
	(r->i) += (xx = len_depth(s + (r->i) + 1));
	return (TRUE);
}

int		ret_virgule(t_arr *r)
{
	if (!((r->t_s)[(r->t_i)] = xmalloc(sizeof(char) * ((r->len) + 1))))
		return (FALSE);
	(r->t_s)[(r->t_i)][0] = '\0';
	(r->t_a)[(r->t_i)] = 0;
	(r->tog) = 1;
	(r->t_s)[++(r->t_i)] = NULL;
	return (TRUE);
}

int		ret_other(t_arr *r, char *s)
{
	int	i_tmp;

	i_tmp = 0;
	while (i_tmp < (r->tog))
	{
		(r->t_s)[(r->t_i) - 1 - i_tmp][(r->t_a)[(r->t_i) - 1 - i_tmp]++] = s[(r->i)];
		(r->t_s)[(r->t_i) - 1 - i_tmp][(r->t_a)[(r->t_i) - 1 - i_tmp]] = '\0';
		i_tmp++;
	}
	return (TRUE);
}

char	**ret_arr(char *s)
{
	t_arr	r;

	if (!ret_init(&r, s))
		return (NULL);
	while (s[(r.i)])
	{
		if (s[(r.i)] == '}')
			break ;
		else if (s[(r.i)] == ',')
		{
			if (!ret_virgule(&r))
				return (NULL);
		}
		else if (s[(r.i)] == '{')
		{
			if (!ret_open_bracket(&r, s))
				return (NULL);
		}
		else
			ret_other(&r, s);
		(r.i)++;
	}
	return (((r.i) == (r.len)) ? NULL : (r.t_s));
}
