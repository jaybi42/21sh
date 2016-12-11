/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_42info.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 15:44:26 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/11 16:03:48 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>

/*
** !!!!!!! DON'T ADD: %s in your info or any %*
*/

t_info g_infos[LEN_INFOS] = {
	{"|", "infos/pipe.txt"},
	{"alias", "infos/alias.txt"}
};

int	exec_42info(int ac, char **argv)
{
	int		i;
	char	*s;

	if (ac == 0)
	{
		ft_dprintf(2, "Usage: 42info \'?\'\n");
		return (0);
	}
	i = -1;
	while (++i < LEN_INFOS)
		if (ft_strcmp(g_infos[i].name, argv[1]) == 0)
		{
			s = file_get_contents(g_infos[i].path);
			if (s != NULL)
			{
				ft_printf(s);
				ft_memdel((void **)&s);
			}
			else
				ft_printf("{red}the path %s was not found!{eoc}\n",
						g_infos[i].path);
			return (0);
		}
	ft_printf("Didn't find '%s'!\n", argv[1]);
	return (0);
}

int	bi_42info(t_av av, t_list **g_env, t_list **g_lenv)
{
	(void)g_env;
	(void)av;
	(void)g_lenv;
	return (exec_42info(av.argc, av.argv));
}
