/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 19:53:32 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/06 17:13:47 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <stdlib.h>

/*
** - clean screen
*/

int			bi_clear(t_av av, t_list **g_env, t_list **g_lenv)
{
	(void)g_env;
	(void)av;
	(void)g_lenv;
	do_term("cl");
	return (0);
}
