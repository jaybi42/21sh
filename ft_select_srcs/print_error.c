/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/12 16:15:07 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/18 12:42:12 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

static t_error const	g_print[7] = {
	{1, " : Env not found.", 0},
	{1, " : Wrong terminal information.", 0},
	{1, " : Can't set terminal information.", 0},
	{1, " : Can't get terminal information.", 0},
	{1, " : Can't get str.", 0},
	{1, " : TOO SMALL windows size.", 0},
	{1, " : Init failed.", 0},
};

int	print_error(char *cmd, int error)
{
	if (g_print[error].cmd == 1)
		ft_putstr_fd(cmd, 2);
	ft_putstr_fd(g_print[error].str, 2);
	if (g_print[error].arg == 1)
		ft_putstr_fd("", 2);
	ft_putchar_fd('\n', 2);
	return (1);
}
