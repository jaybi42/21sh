/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/26 21:20:34 by mseinic           #+#    #+#             */
/*   Updated: 2016/10/26 21:50:57 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

int				count_files(char *path, char *str)
{
	t_aut_info	info;

	info.size = 0;
	if ((info.dirp = opendir(path)) != NULL)
	{
		info.len = ft_strlen(str);
		while ((info.dp = readdir(info.dirp)) != NULL)
		{
			info.tmp = ft_strdup(info.dp->d_name);
			if (info.len <= ft_strlen(info.tmp))
				info.tmp[info.len] = '\0';
			if (auto_my_cmp(info.dp->d_name, info.tmp, str))
				info.size++;
			free(info.tmp);
		}
		closedir(info.dirp);
	}
	return (info.size);
}
