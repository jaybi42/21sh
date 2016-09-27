/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/19 19:08:29 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/27 18:37:11 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
** {is cmd printed, "message", is arg[0] printed} // list start at line 20
*/

static t_error const	g_print[13] = {
	{1, ": too many arguments.", 0},
	{1, ": string not in pwd: ", 1},
	{1, ": no such file or directory: ", 1},
	{1, ": permission denied: ", 1},
	{1, ": Variable name must contain alphanumeric characters.", 0},
	{1, ": Variable name must begin with a letter.", 0},
	{0, "Fork is fucked.", 0},
	{1, ": command not found: ", 1},
	{0, "Pipe error.", 0},
	{1, ": numeric argument required", 0},
	{1, ": Expression Syntax.", 0},
	{1, ": Badly formed number.", 0},
	{1, " : Event not found.", 0},
};

int		print_error(t_av av, int error)
{
	if (g_print[error].cmd == 1)
		ft_putstr(av.cmd);
	ft_putstr(g_print[error].str);
	if (g_print[error].arg == 1)
		ft_putstr(av.arg[0]);
	ft_putchar('\n');
	return (1);
}
