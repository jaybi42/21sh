/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/07 18:54:54 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/11 15:41:56 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void				nparse_init(t_nparse *np, char *expr)
{
	int len;

	len = ft_strlen(expr);
	np->type = xmalloc(sizeof(int) * (len + 1));
	np->begin = xmalloc(sizeof(int) * (len + 1));
	np->end = xmalloc(sizeof(int) * (len + 1));
	np->nb = -1;
	np->failed = TRUE;
}

void				init_cmd(t_av **cmd, size_t len)
{
	(*cmd) = xmalloc(sizeof(t_av));
	(*cmd)->argv = xmalloc(sizeof(char *) * (len + 1));
	(*cmd)->argv_auth = xmalloc(sizeof(int *) * (len + 1));
	(*cmd)->argv_auth[0] = NULL;
	(*cmd)->argv[0] = NULL;
	(*cmd)->argc = 0;
	(*cmd)->bg = FALSE;
	(*cmd)->redirect = xmalloc(sizeof(t_redirect*) * (len + 1));
	(*cmd)->redirect[0] = NULL;
}

char				*xget_string_l(char *s, int l)
{
	int		i;
	char	*ret;

	i = 0;
	ret = xmalloc(sizeof(char *) * (l + 1));
	while (i < l && s[i])
	{
		ret[i] = s[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

int					get_type(char *s)
{
	if (ft_strncmp(s, ";", ft_strlen(";")) == 0)
		return (TYPE_NORMAL);
	if (ft_strncmp(s, "||", ft_strlen("||")) == 0)
		return (TYPE_OR);
	if (ft_strncmp(s, "&&", ft_strlen("&&")) == 0)
		return (TYPE_AND);
	if (ft_strncmp(s, "|", ft_strlen("|")) == 0)
		return (TYPE_PIPE);
	if (ft_strncmp(s, "&", ft_strlen("&")) == 0)
		return (-10);
	else
		return (TYPE_NORMAL);
}

int					*handle_d(t_handle_d_param t, int *t_ind, int *l_ind,
		int len)
{
	int	*x;
	int a;
	int id_x;

	x = xmalloc(sizeof(int *) * (len + 1));
	a = t.np.begin[t.i];
	id_x = 0;
	while (a <= t.np.end[t.i])
	{
		if (is_intouchable(a, t_ind, l_ind))
			x[id_x] = 0;
		else
			x[id_x] = 1;
		id_x++;
		a++;
	}
	return (x);
}
