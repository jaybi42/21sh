/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 20:12:54 by mseinic           #+#    #+#             */
/*   Updated: 2016/10/14 20:19:16 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

char            **ret_match(char *str)
{
	char            **tab;
	char            *tmp;
	char            *path;
	char            *str1;

	str1 = ft_strdup(str);
	tab = NULL;
	tmp = ft_strrchr(str1, '/');
	if (tmp != NULL)
	{
		*tmp = '\0';
		tmp++;
		path = str1;
	}
	else
	{
		path = ".";
		tmp = str1;
	}
	if (ft_strlen(path) == 0)
		path = "/";
	tab = ret_tab(tmp, path);
	free(str1);
	return (tab);
}

char            **command_fnc(char *str)
{
	char        **tab_path;
	char        **tab_ret;
	char        **tmp;
	int         i;

	i = 0;
	tab_ret = NULL;
	tmp = NULL;
	tab_path = ft_strsplit(getenv("PATH"), ':');
	while (tab_path[i] != NULL)
	{
		tmp = append_found(tab_ret, str, tab_path[i]);
		del_tab(tab_ret);
		tab_ret = tab_dup(tmp);
		del_tab(tmp);
		i++;
	}
	del_tab(tab_path);
	return (tab_ret);
}

int             count_files(char *path, char *str)
{
	t_aut_info  info;

	info.size = 0;
	if ((info.dirp = opendir(path)) != NULL)
	{
		info.len = ft_strlen(str);
		while ((info.dp = readdir(info.dirp)) != NULL)
		{
			info.tmp = ft_strdup(info.dp->d_name);
			info.tmp[info.len] = '\0';
			if (ft_strcmp(info.tmp, str) == 0)
				info.size++;
			free(info.tmp);
		}
		closedir(info.dirp);
	}
	return (info.size);
}

char    **ret_tab(char *tmp, char *path)
{
	t_aut_info info;

	info.i = 0;
	info.tab = NULL;
	if ((info.size = count_files(path, tmp)) > 0)
	{
		info.tab = (char **)malloc(sizeof(char *) * (info.size + 1));
		info.len = ft_strlen(tmp);
		info.dirp = opendir(path);
		while ((info.dp = readdir(info.dirp)) != NULL)
		{
			info.tmp = ft_strdup(info.dp->d_name);
			info.tmp[info.len] = '\0';
			if (ft_strcmp(info.tmp, tmp) == 0)
			{
				info.tab[info.i] = ft_strdup(info.dp->d_name);
				info.i++;
			}
			free(info.tmp);
		}
		info.tab[info.i] = NULL;
		closedir(info.dirp);
	}
	return (info.tab);
}

