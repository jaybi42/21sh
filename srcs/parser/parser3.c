/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:34:51 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/06 19:35:02 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int is_intouchable(int i, int *t_ind, int *l_ind)
{
	int id_ind;

	id_ind = 0;
	while(t_ind[id_ind] != -1)
	{
		if ((i >= t_ind[id_ind] && i <= t_ind[id_ind] +
			((l_ind[id_ind] >= 0) ? l_ind[id_ind] : 0)))
			return (1);
		id_ind++;
	}
	return (0);
}

int is_connector(char *tested_s, int i, int *t_ind, int *l_ind)
{
	int id_tab;
	char *s;
	char *tab_connector[] = {";", "||", "&&", "|", "&", NULL};
	int num_tmp;
	int i_tmp;

	id_tab = 0;
	s = tested_s + i;
	if ((!tested_s || !s || !s[0]) || (is_intouchable(i, t_ind, l_ind)))
		return (0);
	while (tab_connector[id_tab])
	{
		if (ft_strncmp(s, tab_connector[id_tab],
				(num_tmp = ft_strlen(tab_connector[id_tab]))) == 0)
		{
			if (num_tmp > 2)
			{
				i_tmp = 0;
				while (i + (++i_tmp) < i + num_tmp)
					if (is_intouchable(i, t_ind, l_ind))
						return (0);
			}
			if (ft_strcmp("&", tab_connector[id_tab]) == 0
			&& i > 0 && !(is_intouchable(i-1, t_ind, l_ind)) && tested_s[i - 1] == '>')
				return (0);
			return (num_tmp);
		}
		id_tab += 1;
	}
	return (0);
}

int		nparse_close(t_nparse *np, int id_close)
{
	if (id_close <= 0)
	{
		print_err("Parsing", "command is null");
		return (FALSE);
	}
	else
		np->end[np->nb] = id_close;
	if (np->begin[np->nb] > np->end[np->nb])
	{
			print_err("Parsing", "command is null");
			return (FALSE);
	}
	return (TRUE);
}

void nparse_extend(t_nparse *np, int id_open)
{
	np->nb += 1;
	np->begin[np->nb] = id_open;
	np->end[np->nb] = -1;
	np->type[np->nb] = 0;
}
