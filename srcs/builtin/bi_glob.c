/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_glob.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 15:50:59 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/06 20:16:29 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <stdlib.h>

/*
** - clean screen
*/

int	ex_glob(int argc, char **argv)
{
	int		i;
	char	**t;
	char	**arg;

	if (argc < 3)
	{
		ft_printf("usage: glob \'string*\' \'string1\' \'string2\' ...\n");
		return (0);
	}
	arg = argv + 2;
	t = ft_globing(argv[1], arg);
	i = -1;
	while (t[++i])
		;
	ft_printf("find {green}%d{eoc} result%c:\n", i, (i > 1) ? 's' : '\0');
	i = -1;
	while (t[++i])
		ft_printf("[%d] \'%s\'\n", i, t[i]);
	return (0);
}

int	bi_glob(t_av av, t_list **g_env, t_list **g_lenv)
{
	(void)g_env;
	(void)av;
	(void)g_lenv;
	return (ex_glob(av.argc + 1, av.argv));
}
