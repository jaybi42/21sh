/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_surgery.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/07 01:04:32 by jguthert          #+#    #+#             */
/*   Updated: 2017/01/07 02:20:42 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void		init_quotes(t_quotes *quotes)
{
	quotes->squote = 0;
	quotes->dquote = 0;
	quotes->bquote = 0;
	quotes->escape = 0;
}

static int		surgery_pack(t_line *l, int *i)
{
	int	j;

	j = *i - 1;
	while (j > 0 && ft_isspace(l->str[j]))
		j--;
	if (l->str[j] != '`')
		return (1);
	if (j == 0)
	{
		l->str = x_strsub(l->str, *i + 1, ft_strlen(l->str));
		*i = -1;
	}
	else if (*i == (int)ft_strlen(l->str) - 1)
	{
		l->str = x_strsub(l->str, 0, j);
		*i = (int)ft_strlen(l->str) - 2;
	}
	else
	{
		l->str = x_strjoin(x_strsub(l->str, 0, j),
							x_strsub(l->str, *i + 1, ft_strlen(l->str)));
		*i = j - 1;
	}
	return (1);
}

static int		print_surgery(char *str)
{
	int		max;
	int		i;

	max = ft_strlen(str);
	i = -1;
	ft_putstr("42sh: command not found: ");
	while (str[++i])
	{
		if (str[i] == '\\' && i < max && str[i + 1] == '`')
			continue ;
		ft_putchar(str[i]);
	}
	return (1);
}

int				parse_surgery(t_quotes *quotes, t_line *l)
{
	int	i;

	i = -1;
	init_quotes(quotes);
	while (l->str[++i])
	{
		if (l->str[i] == '\\')
			quotes->escape ^= 1;
		else if (l->str[i] == '"' && quotes->squote == 0 && quotes->bquote == 0)
			quotes->dquote ^= !quotes->escape;
		else if (l->str[i] == '\'' && quotes->dquote == 0
				&& quotes->bquote == 0)
			quotes->squote ^= !quotes->escape;
		else if (l->str[i] == '`' && quotes->squote == 0)
		{
			if (i > 0 && quotes->escape == 1)
				return (print_surgery(l->str));
			if (quotes->bquote == 1)
				surgery_pack(l, &i);
			quotes->bquote ^= !quotes->escape;
		}
		else
			quotes->escape = 0;
	}
	return (0);
}
