/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_size_tab.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/29 15:57:07 by ibouchla          #+#    #+#             */
/*   Updated: 2016/11/22 22:01:53 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <shell.h>

size_t	ft_size_tab(char **tab2)
{
	size_t	i;

	i = 0;
	while (tab2[i] != NULL)
		++i;
	return (i);
}
