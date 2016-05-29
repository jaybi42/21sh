/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/25 14:42:14 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/29 18:37:38 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <21sh.h>

int			bi_echo(t_av av, t_list **g_env, t_list **l_env)
{
	int		i;
	bool	get_it;

	(void)g_env;
	(void)l_env;
	i = 0;
	av.argc--;
	get_it = 0;
	if (av.argc >= 0 && ft_strcmp(av.arg[0], "-n") == 0)
	{
		get_it = 1;
		i++;
	}
	while (i <= av.argc)
	{
		ft_putstr(av.arg[i++]);
		ft_putchar(' ');
	}
	if (get_it == 0)
		ft_putchar('\n');
	return (0);
}
