/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globexec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 16:57:04 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/06 18:45:21 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	finder_suspension_judgement(t_finder *f, t_globbing *g, char **words)
{
	if ((*(f->p_d_bannish))[(f->a_w)] == 1)
		finder_s_j_soj(f, g, words);
	else
	{
		if (g->list == 1)
			finder_s_j_list(f, g, words);
		else if (g->length_one == 1)
		{
			if (words[(f->a_w)][(*(f->p_d))[(f->a_w)]] == g->exprs[0])
				(f->find) = 1;
		}
		else if (!ft_strncmp(words[(f->a_w)] + (*(f->p_d))[(f->a_w)],
					g->exprs, ft_strlen(g->exprs)))
		{
			(*(f->p_d))[(f->a_w)] += ft_strlen(g->exprs);
			(f->find) = 1;
		}
	}
}

void	finder_not_suspension_judgement(t_finder *f, t_globbing *g,
		char **words)
{
	if (g->length_one == 1)
	{
		if ((size_t)(*(f->p_d))[(f->a_w)] < ft_strlen(words[(f->a_w)]))
			(*(f->p_d))[(f->a_w)]++;
		else
			(*(f->p_d_bannish))[(f->a_w)] = 2;
	}
	else
		(*(f->p_d_bannish))[(f->a_w)] = 1;
	(f->find) = 1;
}

void	finder(t_globbing *g, int **p_d, int **p_d_bannish, char **words)
{
	t_finder f;

	(f.p_d) = p_d;
	(f.p_d_bannish) = p_d_bannish;
	(f.a_w) = -1;
	while (words[++(f.a_w)])
	{
		(f.find) = 0;
		if ((*(f.p_d_bannish))[(f.a_w)] == 2)
			continue ;
		if (g->exprs != NULL)
			finder_suspension_judgement(&f, g, words);
		else if (g->exprs == NULL)
			finder_not_suspension_judgement(&f, g, words);
		if (!(f.find))
			(*(f.p_d_bannish))[(f.a_w)] = 2;
	}
}

int		find_globbing_init(t_norm_fg *f, char **words)
{
	(f->len) = tlen(words);
	if (!((f->d) = xmalloc(sizeof(int) * ((f->len) + 1))))
		return (FALSE);
	if (!((f->d_bannish) = xmalloc(sizeof(int) * ((f->len) + 1))))
		return (FALSE);
	(f->a_w) = -1;
	while (++(f->a_w) < (f->len))
	{
		(f->d_bannish)[(f->a_w)] = 0;
		(f->d)[(f->a_w)] = 0;
	}
	return (TRUE);
}

char	**find_globbing(t_globbing **gs, int a, char **words)
{
	t_norm_fg f;

	if (!find_globbing_init(&f, words))
		return (NULL);
	(f.a_g) = -1;
	while (++(f.a_g) < a)
		finder(gs[(f.a_g)], &(f.d), &(f.d_bannish), words);
	if (!((f.ret) = xmalloc(sizeof(char *) * ((f.len) + 1))))
		return (NULL);
	(f.a_w) = -1;
	a = 0;
	while (words[++(f.a_w)] != NULL)
		if ((f.d_bannish)[(f.a_w)] == 0 || (f.d_bannish)[(f.a_w)] == 1)
			(f.ret)[a++] = x_strdup(words[(f.a_w)]);
	(f.ret)[a] = NULL;
	return ((f.ret));
}
