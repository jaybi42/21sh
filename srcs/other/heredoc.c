/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/30 18:20:40 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/23 18:39:26 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	*clean_return(t_line *l)
{
	ft_strdel(&l->strcpy);
	ft_end(l);
	ft_putchar('\n');
	g_inheredoc = 0;
	return (l->str);
}

static int	init_heredoc(t_line *l, char *prompt)
{
	ft_init_line(l, NULL);
	l->strcpy = NULL;
	l->sizeprompt = ft_strlen(prompt);
	l->line1 = l->largeur - l->sizeprompt;
	g_heredoc = l;
	g_inheredoc = 1;
	return (0);
}

char		*eof_ret(void)
{
	ft_printf("\n");
	return (x_strdup(g_heredoc_expected));
}

char		*get_input(char *prompt)
{
	t_line	l;

	if (init_heredoc(&l, prompt) == 1)
		return (NULL);
	ft_putstr(prompt);
	while (1)
	{
		ft_bzero(l.buffer, 6);
		if (read(0, l.buffer, 6) == -1)
			return (clean_return(&l));
		if (l.buffer[0] == 4 && l.buffer[1] == 0 && l.str[0] == '\0'
				&& g_heredoc_expected)
			return (eof_ret());
		if (l.buffer[0] != 10)
			actions(&l);
		if (l.buffer[0] != 10 && ft_isprint(l.buffer[0]) ==\
				1)
			ft_print_key(&l);
		if (l.buffer[0] == 10)
			break ;
	}
	return (clean_return(&l));
}
