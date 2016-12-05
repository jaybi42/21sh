/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 16:15:12 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/05 16:17:01 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "21sh.h"

#define DEBUG 1

int		init_glob(t_norm_glob *g, char *expr)
{
	if (!expr)
		return (FALSE);
	if (!((g->exprs) = xmalloc(sizeof(int) * (ft_strlen(expr) + 1))))
		return (FALSE);
	(g->exprs) = brace_handler(expr);
	if (!((g->gti) = xmalloc(sizeof(int) * (ft_strlen(expr) + 1))))
		return (FALSE);
	if (!((g->tgs) = xmalloc(sizeof(t_globbing **) * (ft_strlen(expr) + 1))))
		return (FALSE);
	if (!((g->gta) = xmalloc(sizeof(int) * (ft_strlen(expr) + 1))))
		return (FALSE);
	(g->ta) = 0;
	while ((g->exprs)[(g->ta)])
	{
		(g->gti)[(g->ta)] = 0;
		(g->gta)[(g->ta)] = 0;
		if (!((g->tgs)[(g->ta)] = xmalloc(sizeof(t_globbing *)
		* (strlen(expr) + 1))))
			return (FALSE);
		(g->ta)++;
	}
	(g->tgs)[(g->ta)] = NULL;
	(g->ta) = 0;
	return (TRUE);
}

int		exec_glob(t_norm_glob *g, char *expr, char **words)
{
	int x;

	x = 0;
	(g->ta) = 0;
	if (!((g->t) = xmalloc(sizeof(char ***) * (ft_strlen(expr) + 1))))
		return (FALSE);
	x = 0;
	while ((g->tgs)[x] != NULL)
	{
		(g->t)[x] = find_globbing((g->tgs)[x], (g->gta)[x], words);
		if ((g->t)[x] == NULL)
			return (FALSE);
		x++;
	}
	(g->t)[x] = NULL;
	return (TRUE);
}

char	**ft_globing(char *expr, char **words)
{
	t_norm_glob g;

	if (!init_glob(&g, expr))
		return (NULL);
	if (!parse_glob(&g))
		return (NULL);
	if (!exec_glob(&g, expr, words))
		return (NULL);
	return (fusion_tarray((g.t)));
}
