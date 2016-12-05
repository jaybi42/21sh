/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globexec2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 16:25:34 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/05 16:27:30 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void	finder_s_j_soj_list(t_finder *f, t_globbing *g, char **words)
{
	int	i;

	if ((f->debug_list))
		(f->debug_list) = 0;
	i = 0;
	while (g->exprs[i])
	{
		if (g->exprs[i] == words[(f->a_w)][(f->a_w_jugement)])
		{
			if (g->is_looking)
			{
				(*(f->p_d))[(f->a_w)] += (f->a_w_jugement) + 1;
				(*(f->p_d_bannish))[(f->a_w)] = 0;
				(f->find) = 1;
			}
			else
				(*(f->p_d_bannish))[(f->a_w)] = 2;
			(f->leave) = 1;
			break ;
		}
		i++;
	}
}

void	finder_s_j_soj(t_finder *f, t_globbing *g, char **words)
{
//printf("info: last g was in 'soj'. trying to find the next\n");
	(f->debug_list) = 1;
	(f->a_w_jugement) = (*(f->p_d))[(f->a_w)];
	(f->leave) = 0;
	if (words[(f->a_w)][(f->a_w_jugement)] == '\0' && g->exprs[0] == '\0')
		(f->find) = 1;
	while (words[(f->a_w)][(f->a_w_jugement)])
	{
		if (g->list == 1)
			finder_s_j_soj_list(f, g, words);
		else if (ft_strncmp(words[(f->a_w)] + (f->a_w_jugement),
		g->exprs, ft_strlen(g->exprs)) == 0)
		{
			(*(f->p_d))[(f->a_w)] = (f->a_w_jugement) + ft_strlen(g->exprs);
			(*(f->p_d_bannish))[(f->a_w)] = 0;
			//printf("info: find the string |%s|, stopping the search\n", g->exprs);
			(f->find) = 1;
			(f->leave) = 1;
			break ;
		}
		if ((f->leave))
			break ;
		(f->a_w_jugement)++;
	}
	if (!g->is_looking && !(f->find))
		(f->find) = 1;
}
