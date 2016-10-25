/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/30 18:20:40 by jguthert          #+#    #+#             */
/*   Updated: 2016/02/16 17:05:31 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
** prompt heredoc = "heredoc>"
*/

static char	*clear_return(t_line *l)
{
	return (l->str);
}

static int	init_heredoc(t_line *l, char *prompt)
{
	l->str = NULL;
	l->sizeprompt = ft_strlen(prompt);
	l->size = 0;
	l->count = 0;
}

char		*get_input(char *prompt)
{
	t_line	l;

	if (init_heredoc(&l) == 1)
		return (NULL);
	ft_putstr(prompt);
	while (1)
    {
		ft_bzero(l->buffer, 6);
            ft_print_line(l);
        if (read(0, l->buffer, 6) == -1)
            return (clean_ctrl_r(l));
        if (l->buffer[0] != 10)
            actions(l);
        if (l->buffer[0] != 10 && ft_isprint(l->buffer[0]) ==\
			1)
            ft_print_key(l);
        if (l->buffer[0] == 10)
        {
            ft_putchar('\n');
            break ;
        }
    }
	return (clear_return(&l));
}
