/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/26 21:20:34 by mseinic           #+#    #+#             */
/*   Updated: 2016/10/27 16:43:30 by mseinic          ###   ########.fr       */
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
			if (!(info.dp->d_name[0] == '.' && str[0] == '\0'))
			{
				info.tmp = ft_strdup(info.dp->d_name);
				if (info.len <= ft_strlen(info.tmp))
					info.tmp[info.len] = '\0';
				if (auto_my_cmp(info.dp->d_name, info.tmp, str))
					info.size++;
				ft_strdel(&info.tmp);
			}
		}
		closedir(info.dirp);
	}
	return (info.size);
}
