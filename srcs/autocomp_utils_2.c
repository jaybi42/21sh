/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 20:28:06 by mseinic           #+#    #+#             */
/*   Updated: 2016/10/14 20:31:11 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

char            **append_found(char **tab, char *str, char *path)
{
	char        **ret;
	char        **tmp;
	int         i;
	int         j;


	ret = NULL;
	i = 0;
	j = 0;
	tmp = ret_tab(str, path);
	if (tab != NULL)
		while (tab[i] != NULL)
			i++;
	if (tmp != NULL)
		while (tmp[j] != NULL)
			j++;
	ret = (char **)malloc(sizeof(char *) * (i + j + 1));
	i = 0;
	j = 0;
	if (tab != NULL)
	{
		while (tab[i] != NULL)
		{
			ret[i] = ft_strdup(tab[i]);
			i++;
		}
	}
	if (tmp != NULL)
	{
		while (tmp[j] != NULL)
		{
			ret[i + j] = ft_strdup(tmp[j]);
			j++;
		}
	}
	del_tab(tmp);
	ret[i + j] = NULL;
	return (ret);
}
