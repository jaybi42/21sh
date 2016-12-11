/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseruse2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:56:39 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/11 15:43:10 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		char_is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v'
	|| c == '\f' || c == '\r')
		return (TRUE);
	else
		return (FALSE);
}

char	*cpy_a_to_b(char *str, int a, int b)
{
	char	*new_str;
	int		len;
	int		i;

	len = (b - a < 0) ? 0 : b - a;
	if (!(new_str = xmalloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	while (a + i < b)
	{
		new_str[i] = str[a + i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	*find_home(char **env)
{
	int		i;
	char	**t;
	char	*ans;

	i = -1;
	ans = NULL;
	while (env[++i])
		if (ft_strncmp(env[i], "HOME=", 5) == 0)
			break ;
	if (!env[i])
		return (NULL);
	t = x_strsplit(env[i], '=');
	if (t == NULL)
		return (NULL);
	if (t[0] != NULL && t[1] != NULL)
		ans = x_strdup(t[1]);
	return (ans);
}

int		join_string_array_init(t_norm_j_s_a *t, char **a, int *marked_ind)
{
	(t->len) = 0;
	(t->len_ind) = 0;
	(t->i) = -1;
	while (a[++(t->i)])
	{
		(t->len) += ft_strlen(a[(t->i)]) + 1;
		if (marked_ind[(t->i)] == 1)
			(t->len_ind)++;
	}
	if (!((t->ns) = xmalloc(sizeof(char) * ((t->len) + 1))) ||
	!((*(t->t_ind)) = xmalloc(sizeof(int) * ((t->len_ind) + 1))) ||
	!((*(t->l_ind)) = xmalloc(sizeof(int) * ((t->len_ind) + 1))))
		return (FALSE);
	(t->i) = 0;
	(t->len) = 0;
	(t->len_ind) = 0;
	return (TRUE);
}

char	*join_string_array(char **a, int *marked_ind,
		int **t_ind, int **l_ind)
{
	t_norm_j_s_a t;

	t.t_ind = t_ind;
	t.l_ind = l_ind;
	if (!join_string_array_init(&t, a, marked_ind))
		return (FALSE);
	while (a[(t.i)])
	{
		if (marked_ind[(t.i)] == 1)
		{
			(*t_ind)[(t.len_ind)] = (t.len);
			(*l_ind)[(t.len_ind)++] = ft_strlen(a[(t.i)]) - 1;
		}
		x_strjoins(&(t.ns), &(t.len), a[(t.i)], ft_strlen(a[(t.i)]));
		(t.i)++;
	}
	(t.ns)[(t.len)] = '\0';
	(*t_ind)[(t.len_ind)] = -1;
	(*l_ind)[(t.len_ind)] = -1;
	return ((t.ns));
}
