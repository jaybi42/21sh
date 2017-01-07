/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseruse3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 20:49:28 by ibouchla          #+#    #+#             */
/*   Updated: 2017/01/06 18:33:55 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	reset_current(t_parse *p, int index)
{
	p->nb++;
	p->current = EMPTY;
	p->begin[p->nb] = index + 1;
	p->end[p->nb] = 0;
	p->type[p->nb] = EMPTY;
	p->one_arg = FALSE;
	p->quote_activate = FALSE;
}

void	update_new(t_parse *p, int index, int delimiter_index)
{
	p->end[p->nb++] = index;
	p->current = delimiter_index;
	p->type[p->nb] = delimiter_index;
	p->begin[p->nb] = index + ft_strlen(g_delimiter[delimiter_index].name);
	if (g_delimiter[delimiter_index].is_redirection == TRUE)
		p->begin[p->nb] = index;
	p->one_arg = FALSE;
	p->quote_activate = FALSE;
}

void	inject_in_string(char **s1, char *s2, int i_to_insert, int del_length)
{
	int		i;
	int		a;
	char	*ns;
	int		len2;
	int		len;

	len = ft_strlen((*s1));
	len2 = ft_strlen(s2);
	ns = xmalloc(sizeof(char) * (len
	- del_length + len2 + 1));
	a = 0;
	ns[a] = '\0';
	i = -1;
	while (++i < i_to_insert)
		ns[a++] = (*s1)[i];
	i = -1;
	while (++i < len2)
		ns[a++] = s2[i];
	i = i_to_insert + del_length - 1;
	while (++i < len)
		ns[a++] = (*s1)[i];
	ns[a] = '\0';
	(*s1) = ns;
}
