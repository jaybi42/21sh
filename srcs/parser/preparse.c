/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 23:02:52 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/08 23:06:44 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

char	*handle_tilde(char *s, char **env)
{
	char	*home;
	int		i;
	char	**tmpx;
	char	*tmp;
	char	**ret;
	int		find;
	char	*ret_s;
	size_t	len;

	tmp = NULL;
	if (!(tmpx = fstrsplit(s, ft_strlen(s), char_is_whitespace)))
		return (s);
	if (!(home = find_home(env)))
		return (s);
	i = -1;
	while (tmpx[++i])
		;
	ret = xmalloc(sizeof(char *) * (i + 2));
	i = 0;
	find = 0;
	while (tmpx[i])
	{
		tmp = tilde_path(tmpx[i], home);
		if (tmp != NULL)
		{
			find = 1;
			ret[i] = tmp;
		}
		else
			ret[i] = tmpx[i];
		i++;
	}
	ret[i] = NULL;
	if (!find)
		return (s);
	ret_s = NULL;
	len = 0;
	i = -1;
	while (ret[++i])
		len += ft_strlen(ret[i]) + 1;
	ret_s = xmalloc(sizeof(char) * (len + 1));
	i = -1;
	len = 0;
	ret_s[0] = '\0';
	while (ret[++i])
	{
		x_strjoins(&ret_s, &len, ret[i], ft_strlen(ret[i]));
		x_strjoins(&ret_s, &len, " ", 1);
	}
	return (ret_s);
}

char	*decortique_parse(char *expr, size_t l, int **t_ind, int **l_ind)
{
	char		**ts;
	t_parse		*p;
	int			*marked_ind;
	int			i;
	t_output	o;

	marked_ind = xmalloc(sizeof(int) * (l + 1));
	ts = xmalloc(sizeof(char*) * (l + 1));
	ts[0] = NULL;
	p = parse_it2(expr, l, (t_delimiter *)&g_delimiter_quo, 3);
	i = 0;
	while (i < p->nb)
	{
		if (p->type[i] == 2)
		{
			o = shell_exec(cpy_a_to_b(expr, p->begin[i], p->end[i]));
			ts[i] = apply_var(o.string, FALSE);
		}
		else if (p->type[i] == 1)
			ts[i] = cpy_a_to_b(expr, p->begin[i], p->end[i]);
		else
			ts[i] = apply_var(cpy_a_to_b(expr, p->begin[i], p->end[i]), TRUE);
		if (p->type[i] == 0 || p->type[i] == 1)
			marked_ind[i] = 1;
		else
			marked_ind[i] = 0;
		i++;
	}
	ts[i] = NULL;
	return (join_string_array(ts, marked_ind, t_ind, l_ind));
}
