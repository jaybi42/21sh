/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:47:07 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/06 15:34:50 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static void		init_quotes(t_quotes *quotes)
{
	quotes->squote = 0;
	quotes->dquote = 0;
	quotes->bquote = 0;
	quotes->escape = 0;
}

static int		check_quotes(t_quotes *quotes, char *line)
{
	while (*line)
	{
		if (*line == '\\')
			quotes->escape ^= 1;
		else if (*line == '"' && quotes->squote == 0 && quotes->bquote == 0)
			quotes->dquote ^= !quotes->escape;
		else if (*line == '\'' && quotes->dquote == 0 && quotes->bquote == 0)
			quotes->squote ^= 1;
		else if (*line == '`' && quotes->squote == 0)
			quotes->bquote ^= !quotes->escape;
		else
			quotes->escape = 0;
		line++;
	}
	return (quotes->bquote && quotes->squote && quotes->dquote);
}

static int		concat_str(t_line *l, char *input)
{
	char	*tmp;

	tmp = ft_strjoin(l->str, input);
	ft_strdel(&input);
	if (tmp == NULL)
		return (1);
	ft_strdel(&l->str);
	l->str = tmp;
	return (0);
}

static int		close_quotes(t_quotes *q, t_line *l)
{
	char	*input;

	input = NULL;
	if (q->bquote == 1)
		input = get_input("bquote>");
	else if (q->squote == 1)
		input = get_input("quote>");
	else if (q->dquote == 1)
		input = get_input("dquote>");
	if (input == NULL)
		return (1);
	return (concat_str(l, input));
}

int				parse_quote(t_line *l)
{
	t_quotes	quotes;

	init_quotes(&quotes);
	while (check_quotes(&quotes, l->str) == 1)
	{
		if (close_quotes(&quotes, l) == 1)
			return (1);
	}
	return (0);
}
