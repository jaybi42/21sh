/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:47:07 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/25 19:06:48 by jguthert         ###   ########.fr       */
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

static void		check_quotes(t_quotes *quotes, char *line)
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
}

static int		is_quotes_ok(t_quotes quotes)
{
	if (quotes.squote == 1)
		ft_putendl_fd("Unmatched '.", 2);
	else if (quotes.dquote == 1)
		ft_putendl_fd("Unmatched \".", 2);
	else if (quotes.bquote == 1)
		ft_putendl_fd("Unmatched `.", 2);
	else
		return (1);
	return (0);
}

int				parse_quote(char *line)
{
	t_quotes	quotes;

	init_quotes(&quotes);
	check_quotes(&quotes, line);
	return (is_quotes_ok(quotes));
}
