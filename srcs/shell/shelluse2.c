/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelluse2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:55:03 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/06 19:55:38 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	print_err(char *err, char *what)
{
	ft_dprintf(2, "%s: %s: %s\n", NAME, err, what);
}
