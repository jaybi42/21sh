/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uncatch_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/18 19:27:56 by malaine           #+#    #+#             */
/*   Updated: 2016/12/20 18:29:48 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <signal.h>

void		uncatch_signal(void)
{
	int i;

	i = 4;
	while (i++ < 28)
		signal(i, SIG_IGN);
}
