/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ret_glob.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 21:29:12 by mseinic           #+#    #+#             */
/*   Updated: 2017/01/06 19:32:34 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

char		*ret_glob(char *str)
{
	char	*tmp;
	char	**ret;
	char	*my_str;

	tmp = x_strdup(str);
	if (ft_strchr(tmp, '*') != NULL || ft_strchr(tmp, '{') != NULL ||
			ft_strchr(tmp, '[') != NULL || ft_strchr(tmp, '?') != NULL)
	{
		ret = ret_match(tmp);
		if (ret == NULL)
			return (NULL);
		else
		{
			my_str = x_strdup(ret[0]);
			del_tab(ret);
			return (my_str);
		}
	}
	return (NULL);
}
