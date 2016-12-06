/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ret_glob.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 21:29:12 by mseinic           #+#    #+#             */
/*   Updated: 2016/12/06 21:37:35 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

char		*ret_glob(char *str)
{
	char	*tmp;
	char	**ret;

	tmp = x_strdup(str);
	if (ft_strchr(tmp, '*') != NULL || ft_strchr(tmp, '{') != NULL ||
			ft_strchr(tmp, '[') != NULL || ft_strchr(tmp, '?') != NULL)
	{
		ret = ret_match(tmp);
		if (ret == NULL)
			return (NULL);
		else
			return (ret[0]);
	}
	return (NULL);
}
