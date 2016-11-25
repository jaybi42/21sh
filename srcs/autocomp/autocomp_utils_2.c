/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 20:28:06 by mseinic           #+#    #+#             */
/*   Updated: 2016/11/06 19:29:01 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

static void			fill_in(char **t, char **fill, int i)
{
	int				j;

	j = 0;
	while (fill[j] != NULL)
	{
		t[i + j] = ft_strdup(fill[j]);
		j++;
	}
}

char				**append_found(char **t, char *str, char *path)
{
	char			**ret;
	char			**tmp;
	int				i;
	int				j;

	ret = NULL;
	i = 0;
	j = 0;
	tmp = ret_tab(str, path);
	if (t != NULL)
		i = tab_size(t);
	if (tmp != NULL)
		j = tab_size(tmp);
	if (!(ret = (char **)malloc(sizeof(char *) * (i + j + 1))))
		return (NULL);
	if (t != NULL)
		fill_in(ret, t, 0);
	if (tmp != NULL)
		fill_in(ret, tmp, i);
	del_tab(tmp);
	ret[i + j] = NULL;
	return (ret);
}

static void			my_get_path(char **tmp, char **path, char *str1)
{
	if ((*tmp = ft_strrchr(str1, '/')) != NULL)
	{
		**tmp = '\0';
		*tmp = *tmp + 1;
		*path = str1;
	}
	else
	{
		*path = ".";
		*tmp = str1;
	}
}

char				**ret_match(char *str)
{
	char			**t;
	char			*tmp;
	char			*path;
	char			*str1;

	str1 = ft_strdup(str);
	t = NULL;
	my_get_path(&tmp, &path, str1);
	if (ft_strlen(path) == 0)
		path = "/";
	if (ft_strchr(tmp, '*') != NULL || ft_strchr(tmp, '{') != NULL ||
			ft_strchr(tmp, '[') != NULL || ft_strchr(tmp, '?') != NULL)
		t = ret_globing(tmp, path);
	else
		t = ret_tab(tmp, path);
	ft_strdel(&str1);
	return (t);
}

char				**command_fnc(char *str)
{
	char			**tab_path;
	char			**tab_ret;
	char			**tmp;
	int				i;

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
