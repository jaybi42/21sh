/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:47:07 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/09 14:53:39 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

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

static void		is_quotes_ok(t_quotes quotes)
{
	if (quotes.squote == 1)
		ft_putendl_fd("\033[31m   Unmatched '.\033[0m", 2);
	else if (quotes.dquote == 1)
		ft_putendl_fd("\033[31m   Unmatched \".\033[0m", 2);
	else if (quotes.bquote == 1)
		ft_putendl_fd("\033[31m   Unmatched `.\033[0m", 2);
	else
		ft_putendl_fd("\033[32m   Everything is ok\033[0m", 2);
}

static void		parse_quote(char *line)
{
	t_quotes	quotes;

	init_quotes(&quotes);
	check_quotes(&quotes, line);
	is_quotes_ok(quotes);
}

static void		is_ok(char *line, int i, bool *color)
{
	if (ft_strchr(line, '1') != NULL)
		*color = 1;
	else if (ft_strchr(line, '0') != NULL)
		*color = 0;
	if (*color == 0)
		ft_putstr_fd("\033[31m", 2);
	else
		ft_putstr_fd("\033[32m", 2);
	ft_putchar_fd('[', 2);
	ft_putstr_fd("test", 2);
	ft_putstr_fd(" ", 2);
	ft_putnbr_fd(i, 2);
	ft_putchar_fd(']', 2);
	ft_putstr_fd("\033[0m", 2);
}

int			main_quote(void)
{
	int		fd;
	char	*line;
	int		i;
	bool	color;

	i = 0;
	color = 1;
	fd = open("quote.test", O_RDONLY);
	if (fd == -1)
		exit (1);
	while (get_next_line(fd, &line) > 0)
	{
		is_ok(line, i, &color);
		parse_quote(line);
		i++;
	}
	if (close(fd) == -1)
		exit (1);
	exit (0);
}
