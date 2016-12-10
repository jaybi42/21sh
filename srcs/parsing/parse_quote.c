/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:47:07 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/10 19:47:21 by ibouchla         ###   ########.fr       */
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
	init_quotes(quotes);
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
	return (!quotes->bquote && !quotes->squote && !quotes->dquote);
}

static int		concat_str(t_line *l, char *input)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strjoin("\n", input);
	input = NULL;
	if (tmp == NULL)
		return (1);
	tmp2 = x_strjoin(l->str, tmp);
	ft_strdel(&tmp);
	if (tmp2 == NULL)
		return (1);
	l->str = tmp2;
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

	while (check_quotes(&quotes, l->str) == 0)
	{
		if (close_quotes(&quotes, l) == 1)
			return (1);
	}
	return (0);
}
