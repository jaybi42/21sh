/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 19:53:32 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/19 17:58:26 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>

int			bi_exit(t_av av, t_list **g_env, t_list **l_env)
{
	(void)g_env;
	(void)l_env;
	if (av.argc == 0)
		exit(0);
	exit(ft_atoi(av.arg[0]));
}
