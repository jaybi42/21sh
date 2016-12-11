/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 19:53:32 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/06 17:14:02 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <stdlib.h>

/*
** - Clean g_env and g_lenv
** - return -1 to exit shell
*/

int			bi_exit(t_av av, t_list **g_env, t_list **g_lenv)
{
	if (*av.arg != NULL && !ft_strisnum(*av.arg))
	{
		if (ft_isdigit(*av.arg[0]))
			print_error(av, 11);
		else
			print_error(av, 10);
		return (0);
	}
	ft_lstdel(g_env, free_env);
	ft_lstdel(g_lenv, free_env);
	ret_exit(SET, (av.argv[1] != NULL) ? ft_atoi(av.argv[1]) : 0);
	*g_exit = (av.argv[1] != NULL) ? (unsigned char)ft_atoi(av.argv[1]) : 0;
	return (0);
}
