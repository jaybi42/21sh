/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_first_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/25 18:45:24 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/25 18:46:12 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_first_word(char *str)
{
	int i;

	i = 0;
	while (*str != '\0' && ft_isblank(*str) == 1)
		str++;
	while (str[i] != '\0' && ft_isblank(str[i]) == 0)
		i++;
	return (ft_strndup(str, i));
}
