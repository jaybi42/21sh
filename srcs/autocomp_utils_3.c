/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/26 20:30:39 by mseinic           #+#    #+#             */
/*   Updated: 2016/10/26 21:57:33 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"

static void		ft_copy_tab(char *tab_ret, char *data, int j, int *k)
{
	while (data[j] != '\0')
	{
		tab_ret[*k] = data[j];
		j = j + 1;
		*k = *k + 1;
	}
}

void			insert_in_string(t_ft_info *info, t_line *l, char *data)
{
	char		*tmp;

	info->k = info->size + ft_strlen(data) + 1;
	if (!(tmp = ft_strnew(info->k)))
		return ;
	info->k = 0;
	while (info->k < info->index)
	{
		tmp[info->k] = info->copy[info->k];
		info->k++;
	}
	ft_copy_tab(tmp, data, 0, &info->k);
	ft_copy_tab(tmp, info->copy, info->index, &info->k);
	tmp[info->k] = '\0';
	ft_strdel(&(l->str));
	l->str = tmp;
	l->count = info->index + ft_strlen(data);
	l->size = ft_strlen(l->str);
}

void			insert_ontop_string(t_ft_info *info, t_line *l, char *data)
{
	char		*tmp;
	int			size;

	info->k = info->index;
	while (info->k > 0 && info->copy[info->k - 1] != ' ')
		info->k--;
	info->k = info->index - info->k;
	size = ft_strlen(info->copy) + ft_strlen(data) + 1 - info->k;
	if (!(tmp = ft_strnew(size)))
		return ;
	size = 0;
	info->k = info->index - info->k;
	while (size < info->k)
	{
		tmp[size] = info->copy[size];
		size++;
	}
	ft_copy_tab(tmp, data, 0, &size);
	ft_copy_tab(tmp, info->copy, info->index, &size);
	tmp[size] = '\0';
	ft_strdel(&(l->str));
	l->str = tmp;
	l->count = info->k + ft_strlen(data);
	l->size = ft_strlen(l->str);
}

int				test_if_fnc(int i, char *str, int *glob)
{
	char		*ptr;

	if ((ptr = ft_strchr(str, '*')) != NULL || ft_strchr(str, '{') != NULL ||
			ft_strchr(str, '[') != NULL || ft_strchr(str, '?') != NULL)
	{
		*glob = 1;
		return (0);
	}
	if (str[i] == '/' || str[i] == '.')
		return (0);
	if (i == 0)
		return (1);
	else
	{
		while (i > 0 && str[i - 1] == ' ')
			i--;
		if (i == 0)
			return (1);
		else if (str[i - 1] == '|' || str[i - 1] == ';')
			return (1);
	}
	return (0);
}

void			init(t_ft_info *info, t_line *l)
{
	info->count = l->count;
	info->size = ft_strlen(l->str);
	info->copy = ft_strdup(l->str);
	info->copy2 = ft_strdup(l->str);
	info->index = l->count;
	info->complete = ft_strnew(info->size);
	info->aux = ft_strnew(info->size);
	while (info->index > 0 && info->copy[info->index - 1] != ' ')
		info->index--;
	info->function = test_if_fnc(info->index, info->copy, &(info->globing));
	while (info->copy[info->index] != '\0' && info->copy[info->index] != ' ')
	{
		info->complete[info->k] = info->copy[info->index];
		info->index++;
		info->k++;
	}
	info->complete[info->k] = '\0';
	ft_strcpy(info->aux, info->complete);
}
