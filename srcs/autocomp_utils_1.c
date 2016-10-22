/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 20:12:54 by mseinic           #+#    #+#             */
/*   Updated: 2016/10/22 20:05:42 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

char            **ret_match(char *str)
{
	char            **tab;
	char            *tmp;
	char            *path;
	char            *str1;
	char			**tab2;

	str1 = ft_strdup(str);
	tab = NULL;
	tab2 = NULL;
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
	if (ft_strchr(tmp, '*') != NULL)
	{
		//printf("[%s] ",tmp);
		tab = ret_tab("", path);
		for (int i = 0; tab[i] != NULL; i++)
			printf("[%s] ", tab[i]);
		if (tab != NULL)
			tab2 = ft_globing(tmp, tab);
		
		int	size = 0;
		int	n = 0;
		int	test = 0;

		if (ft_strcmp(path, "."))
		{
			test = 1;
			n = ft_strlen(path) + 1;
		}
		for (int i = 0; tab2[i] != NULL; i++)
		{
			size += (ft_strlen(tab2[i]) + n + 1);
		}
		char **ret = (char**)malloc(sizeof(char *) * 2);
		ret[0] = (char *)malloc(sizeof(char) * (size + 1 - (n - 1)));
		ft_bzero(ret[0], size + 1 - (n - 1));
		for (int i = 0; tab2[i] != NULL; i++)
		{
			if (tab2[i][0] != '.')
			{
				if (test)
				{
					ft_strcat(ret[0], path);
					ft_strcat(ret[0], "/");
				}
				ft_strcat(ret[0], tab2[i]);
				ft_strcat(ret[0], " ");
			}
		}

			printf("[%s] ", ret[0]);
		ret[1] = NULL;
		return (ret);
	}
	else
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

