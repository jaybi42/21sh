/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storage_all_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/28 22:16:21 by ibouchla          #+#    #+#             */
/*   Updated: 2016/11/17 18:20:33 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

t_av **updated(t_av **av)
{
	int i;

	i = 0;
	while (av[i] != NULL)
	{
		if (av[i]->argv != NULL)
		{
			if (av[i]->argv[0] != NULL)
			{
				av[i]->cmd = av[i]->argv[0];
				av[i]->arg = copy_array_begin(1, av[i]->argv);
				av[i]->argc--;
			}
			else
			{
				av[i]->cmd = NULL;
				av[i]->arg = NULL;
			}
		}
		else
		{
			av[i]->cmd = NULL;
			av[i]->arg = NULL;
		}
		if (g_debug)
		{
		for (int x = 0; av[i]->argv[x]; x++)
		{
			dprintf(2, "cmd %.*s %d argv[%d] : %%",(i + 1)*2,
			"---------------------------------------", i, x);
			for (int b = 0; av[i]->argv[x][b]; b++)
			{
				if (av[i]->argv_auth[x][b] == 1)
					ft_dprintf(2, "{green}%c{eoc}", av[i]->argv[x][b]);
				else
					ft_dprintf(2, "{red}%c{eoc}", av[i]->argv[x][b]);
			}
			ft_dprintf(2, "%%\n");
		}
		}
		i++;
	}
	(g_debug) ? ft_dprintf(2, "{yellow}----------------------{eoc}\n") : 0;
	return (av);
}


/*
** check the var from the env!
*/
t_av **nparse(char *expr, int *t_ind, int *l_ind)
{
	t_av **cmds;
	t_nparse np;

	np = parse(expr, t_ind, l_ind);
	if (np.failed == TRUE)
	{
		(g_debug) ? ft_dprintf(2, "STEP X: {red}stop due to an error{eoc}\n") : 0;
		return (NULL);
	}
	(g_debug) ? ft_dprintf(2, "STEP 2: {green}converting{eoc}\n") : 0;
	cmds = convert_parse(expr, np, t_ind, l_ind);
	convert_other(&cmds);
	return (cmds);
}

t_av	**parse_commands(char *expr)
{
	int			*t_ind;
	int			*l_ind;
	t_av **cmds;
	int id_cmd;

	(g_debug) ? ft_dprintf(2, "{yellow}----------------------{eoc}\n") : 0;
	id_cmd = -1;
	while(is_whitespace(expr[++id_cmd]));
	if ((size_t)id_cmd >= ft_strlen(expr))
		return (NULL);
	if (!(cmds = xmalloc(sizeof(t_av **) * (ft_strlen(expr) + 1))))
		return (NULL);
	cmds[(id_cmd = 0)] = NULL;
	expr = decortique_parse(expr, ft_strlen(expr), &t_ind, &l_ind);
	if (g_debug)
		print_step1(expr, t_ind, l_ind);
	if (!(cmds = nparse(expr, t_ind, l_ind)))
		return (NULL);
	return (updated(cmds));
}
