/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/07 18:50:10 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/08 20:45:21 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int		in_cmd_is_intouchable(t_av *cmd, int id_argv, int i)
{
	if ((cmd->argv_auth) != NULL)
		if ((cmd->argv_auth[id_argv]) != NULL)
			if ((cmd->argv_auth[id_argv][i]) == 1)
				return (0);
	return (1);
}

void	convert_other(t_av ***cmds)
{
	int i;

	i = 0;
	while ((*cmds)[i])
	{
		get_redirect(&(*cmds)[i]);
		i++;
	}
}
