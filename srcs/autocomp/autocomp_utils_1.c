/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 20:12:54 by mseinic           #+#    #+#             */
/*   Updated: 2016/12/18 19:44:28 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"
#include <sys/stat.h>

static void			ft_skip(char **ret, char *path, char **tab2, char *tmp)
{
	int		i;

	i = 0;
	while (tab2[i] != NULL)
	{
		if (tmp[0] != '.')
			while (tab2[i] != NULL && tab2[i][0] == '.')
				i++;
		else
			while (tab2[i] != NULL && tab2[i][0] != '.')
				i++;
		if (tab2[i] == NULL)
			break ;
		if (ft_strcmp(path, ".") != 0)
		{
			ft_strcat(ret[0], path);
			if (ft_strcmp(path, "/") != 0)
				ft_strcat(ret[0], "/");
		}
		ft_strcat(ret[0], tab2[i]);
		ft_strcat(ret[0], " ");
		i++;
	}
}

static char			**get_table(char **tab2, int n, char *path, char *tmp)
{
	char	**ret;
	int		i;
	int		size;

	i = -1;
	size = 0;
	while (tab2[++i] != NULL)
		size += (ft_strlen(tab2[i]) + n + 1);
	i = 0;
	ret = (char**)malloc(sizeof(char *) * 2);
	ret[0] = NULL;
	if ((ret[0] = ft_strnew(size)) != NULL)
		ft_skip(ret, path, tab2, tmp);
	ret[1] = NULL;
	return (ret);
}

char				**ret_globing(char *tmp, char *path)
{
	char	**t;
	char	**tab2;
	int		n;
	char	**ret;

	init_var(&n, &t, &tab2, &ret);
	if (tmp[0] == '.')
		t = ret_tab(".", path);
	else
		t = ret_tab("", path);
	if (t != NULL)
		get_slashes(t);
	if (t != NULL)
		tab2 = ft_globing(tmp, t);
	if (tab2 == NULL)
		return (del_return_null(&t));
	if (tab2[0] == NULL)
		return (del_return_null(&t));
	if (ft_strcmp(path, ".") != 0 && ft_strcmp(path, "/") != 0)
		n = ft_strlen(path) + 1;
	else
		n = 2;
	ret = get_table(tab2, n, path, tmp);
	del_tab(t);
	return (ret);
}

void				just_norme(t_aut_info *info, char *tmp, char *path)
{
	if (!(info->dp->d_name[0] == '.' && tmp[0] == '\0'))
	{
		info->tmp = ft_strdup(info->dp->d_name);
		if (info->len <= ft_strlen(info->tmp))
			info->tmp[info->len] = '\0';
		if (auto_my_cmp(info->dp->d_name, info->tmp, tmp))
		{
			if (verify_if_dir(info, path))
				add_slash(info);
			else
				info->tab_ret[info->i++] = ft_strdup(info->dp->d_name);
		}
		ft_strdel(&info->tmp);
	}
}

char				**ret_tab(char *tmp, char *path)
{
	t_aut_info info;

	ft_bzero(&info, sizeof(t_aut_info));
	if ((info.size = count_files(path, tmp)) > 0)
	{
		if (!(info.tab_ret = (char **)malloc(sizeof(char *) * (info.size + 1))))
			return (NULL);
		info.len = ft_strlen(tmp);
		info.dirp = opendir(path);
		while ((info.dp = readdir(info.dirp)) != NULL)
			just_norme(&info, tmp, path);
		info.tab_ret[info.i] = NULL;
		closedir(info.dirp);
	}
	return (info.tab_ret);
}
