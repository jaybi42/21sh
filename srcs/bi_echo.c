/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/25 14:42:14 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/25 14:56:22 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <21sh.h>

int			bi_echo(t_av av, t_list **g_env, t_list **l_env)
{
	int		i;

	(void)g_env;
	(void)l_env;
	i = 0;
	av.argc--;
	while (i < av.argc)
	{
		ft_putstr(av.arg[i++]);
		ft_putchar(' ');
	}
	ft_putstr(av.arg[i]);
	ft_putchar('\n');
	return (0);
}
