/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globexec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 16:28:02 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/05 16:33:21 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void	finder_s_j_list(t_finder *f, t_globbing *g, char **words)
{
	int	i;

	i = 0;
	while (g->exprs[i])
	{
		if (g->exprs[i] == words[(f->a_w)][(*(f->p_d))[(f->a_w)]])
		{
			if (g->is_looking)
			{
				(*(f->p_d))[(f->a_w)] += 1;
				(f->find) = 1;
			}
			else
				(*(f->p_d_bannish))[(f->a_w)] = 2;
			break ;
		}
		i++;
	}
	if (!g->is_looking && !(f->find))
		(f->find) = 1;
}

void	finder_suspension_judgement(t_finder *f, t_globbing *g, char **words)
{
	if ((*(f->p_d_bannish))[(f->a_w)] == 1)
		finder_s_j_soj(f, g, words);
	else
	{
		//printf("try to force! with %d - %c\n", (*(f->p_d))[(f->a_w)], words[(f->a_w)][(*(f->p_d))[(f->a_w)]]);
		if (g->list == 1)
			finder_s_j_list(f, g, words);
		else if (g->length_one == 1)
		{
			if (words[(f->a_w)][(*(f->p_d))[(f->a_w)]] == g->exprs[0])
				(f->find) = 1;
		}
		else if (!strncmp(words[(f->a_w)] + (*(f->p_d))[(f->a_w)],
		g->exprs, strlen(g->exprs)))
		{
			(*(f->p_d))[(f->a_w)] += strlen(g->exprs);
			//printf("info: nor: (f->find) %s\n", g->exprs);
			(f->find) = 1;
		}
	}
}

void	finder_not_suspension_judgement(t_finder *f,
		t_globbing *g, char **words)
{
	if (g->length_one == 1)
	{
		if ((size_t)(*(f->p_d))[(f->a_w)] < ft_strlen(words[(f->a_w)]))
		{
			//printf("info: ???: find a ?, %d|%c|\n",(*(f->p_d))[(f->a_w)], words[(f->a_w)][(*(f->p_d))[(f->a_w)]]);
			(*(f->p_d))[(f->a_w)]++;
		}
		else
			(*(f->p_d_bannish))[(f->a_w)] = 2;
	}
	else
		(*(f->p_d_bannish))[(f->a_w)] = 1;//mean "in suspension of jugement"
	(f->find) = 1;
}

void	finder(t_globbing *g, int **p_d, int **p_d_bannish, char **words)
{
	t_finder	f;

	(f.a_w) = -1;
	(f.p_d) = p_d;
	(f.p_d_bannish) = p_d_bannish;
	while (words[++(f.a_w)])
	{
		(f.find) = 0;
		if (*(f.p_d_bannish)[(f.a_w)] == 2)
			continue ;
		if (g->exprs != NULL)
			finder_suspension_judgement(&f, g, words);
		else if (g->exprs == NULL)
			finder_not_suspension_judgement(&f, g, words);
		if (!(f.find))
			(*(f.p_d_bannish))[(f.a_w)] = 2;
	}
}

char	**find_globbing(t_globbing **gs, int a, char **words)
{
	int		*d;
	int		*d_bannish;
	int		len;
	int		a_g;
	int		a_w;
	char	**ret;

	len = tlen(words);
	if (!(d = xmalloc(sizeof(int) * (len + 1))))
		return (NULL);
	if (!(d_bannish = xmalloc(sizeof(int) * (len + 1))))
		return (NULL);
	a_w = -1;
	while (++a_w < len)
	{
		d_bannish[a_w] = 0;
		d[a_w] = 0;
	}
	a_g = -1;
	while (++a_g < a)
		finder(gs[a_g], &d, &d_bannish, words);
	if (!(ret = xmalloc(sizeof(char *) * (tlen(words) + 1))))
		return (NULL);
	a_w = -1;
	a = 0;
	while (words[++a_w] != NULL)
		if (d_bannish[a_w] == 0 || d_bannish[a_w] == 1)
			ret[a++] = x_strdup(words[a_w]);
	ret[a] = NULL;
	return (ret);
}
