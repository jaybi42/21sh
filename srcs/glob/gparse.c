/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gparse.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 16:50:38 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/05 16:54:14 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int		parse_glob_rect(t_norm_glob *g)
{
	int	tmp_len;

	tmp_len = 0;
	if (((g->g_tmp) = quarter((g->exprs)[(g->ta)] + (g->gti)[(g->ta)] + 1,
	&tmp_len)) != NULL)
	{
		(g->tgs)[(g->ta)][(g->gta)[(g->ta)]++] = (g->g_tmp);
		(g->gti)[(g->ta)] += tmp_len + 1;
	}
	else if (((g->g_tmp) = rec_g((g->exprs)[(g->ta)],
	&(g->gti)[(g->ta)], 0)) != NULL)
		(g->tgs)[(g->ta)][(g->gta)[(g->ta)]++] = (g->g_tmp);
	else
		return (FALSE);
	return (TRUE);
}

int		parse_glob_star(t_norm_glob *g)
{
	while ((g->exprs)[(g->ta)][(g->gti)[(g->ta)] + (g->c)] == '*')
		(g->c)++;
	if (!((g->tgs)[(g->ta)][(g->gta)[(g->ta)]] = xmalloc(sizeof(t_globbing))))
		return (FALSE);
	(g->tgs)[(g->ta)][(g->gta)[(g->ta)]]->is_looking = 1;
	(g->tgs)[(g->ta)][(g->gta)[(g->ta)]]->length_one = 0;
	(g->tgs)[(g->ta)][(g->gta)[(g->ta)]]->list = 0;
	(g->tgs)[(g->ta)][(g->gta)[(g->ta)]++]->exprs = NULL;
	(g->gti)[(g->ta)] += (g->c) - 1;
	return (TRUE);
}

int		parse_glob_imark(t_norm_glob *g)
{
	if (!((g->tgs)[(g->ta)][(g->gta)[(g->ta)]] = xmalloc(sizeof(t_globbing))))
		return (FALSE);
	(g->tgs)[(g->ta)][(g->gta)[(g->ta)]]->is_looking = 1;
	(g->tgs)[(g->ta)][(g->gta)[(g->ta)]]->length_one = 1;
	(g->tgs)[(g->ta)][(g->gta)[(g->ta)]]->list = 0;
	(g->tgs)[(g->ta)][(g->gta)[(g->ta)]++]->exprs = NULL;
	return (TRUE);
}

int		parse_glob_globbing(t_norm_glob *g)
{
	while ((g->exprs)[(g->ta)][(g->gti)[(g->ta)]])
	{
		(g->c) = 0;
		if ((g->exprs)[(g->ta)][(g->gti)[(g->ta)]] == '[')
		{
			if (!parse_glob_rect(g))
				return (FALSE);
		}
		else if ((g->exprs)[(g->ta)][(g->gti)[(g->ta)]] == '*')
		{
			if (!parse_glob_star(g))
				return (FALSE);
		}
		else if ((g->exprs)[(g->ta)][(g->gti)[(g->ta)]] == '?')
		{
			if (!parse_glob_imark(g))
				return (FALSE);
		}
		else if (((g->g_tmp) = rec_g((g->exprs)[(g->ta)],
		(g->gti) + (g->ta), 1)) != NULL)
			(g->tgs)[(g->ta)][(g->gta)[(g->ta)]++] = (g->g_tmp);
		else
			return (FALSE);
		(g->gti)[(g->ta)]++;
	}
	return (TRUE);
}

int		parse_glob(t_norm_glob *g)
{
	while ((g->exprs)[(g->ta)])
	{
		if (!parse_glob_globbing(g))
			return (FALSE);
		(g->tgs)[(g->ta)][(g->gta)[(g->ta)]] = xmalloc(sizeof(t_globbing));
		if (!((g->tgs)[(g->ta)][(g->gta)[(g->ta)]]))
			return (FALSE);
		(g->tgs)[(g->ta)][(g->gta)[(g->ta)]]->exprs = x_strdup("");
		(g->tgs)[(g->ta)][(g->gta)[(g->ta)]]->is_looking = 1;
		(g->tgs)[(g->ta)][(g->gta)[(g->ta)]]->list = 0;
		(g->tgs)[(g->ta)][(g->gta)[(g->ta)]]->length_one = 1;
		(g->tgs)[(g->ta)][++(g->gta)[(g->ta)]] = NULL;
		(g->ta)++;
	}
	(g->tgs)[(g->ta)] = NULL;
	return (TRUE);
}
