/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_1_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 20:13:08 by mseinic           #+#    #+#             */
/*   Updated: 2016/12/05 20:14:53 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"
#include <sys/stat.h>

void				add_slash(t_aut_info *info)
{
	char			*tmp;

	tmp = ft_strnew(ft_strlen(info->dp->d_name) + 1);
	ft_strcat(tmp, info->dp->d_name);
	ft_strcat(tmp, "/");
	info->tab_ret[info->i++] = ft_strdup(tmp);
	ft_strdel(&tmp);
}

int					verify_if_dir(t_aut_info *info, char *path)
{
	char			*str;

	str = ft_strnew(ft_strlen(info->dp->d_name) + ft_strlen(path) + 1);
	if (path[0] == '/' && ft_strlen(path) == 1)
	{
		ft_strcat(str, "/");
		ft_strcat(str, info->dp->d_name);
	}
	else
	{
		ft_strcat(str, path);
		ft_strcat(str, "/");
		ft_strcat(str, info->dp->d_name);
	}
	if (is_dir_file(str))
	{
		ft_strdel(&str);
		return (1);
	}
	ft_strdel(&str);
	return (0);
}
