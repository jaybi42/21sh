/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:22:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/26 16:52:37 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include "autocomp.h"
#include <stdio.h> // <<<

static t_actions const  g_actions[1] = {
	{NULL, {9, 0, 0, 0, 0, 0}, "TAB"},
};

typedef struct s_ft_info
{	
	char		*complete;
	int			size;
	int			complete_size;
	char		*aux;
	char		**tab_ret;
	int			function;
	int			index;
	char		*copy;
	int			k;
	int			globing;
}				t_ft_info;



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
	char	*ptr;

	if ((ptr = ft_strchr(str, '*')) != NULL || ft_strchr(str, '{') != NULL ||
			 ft_strchr(str, '[') != NULL || ft_strchr(str, '?') != NULL)
	{
		*glob = 1;
		return (0);
	}
	if (str[i] == '/')
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
	info->size = ft_strlen(l->str);
	info->copy = ft_strdup(l->str);
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

void			get_prepared(t_ft_info *info, t_line *l)
{
	char	*ptr;

	init(info, l);
	if (!info->function)
		info->tab_ret = ret_match(info->complete);
	else
		info->tab_ret = command_fnc(info->complete);
	if ((ptr = ft_strrchr(info->complete, '/')) != NULL)
	{
		ptr++;
		ft_bzero(info->complete, info->size);
		ft_strcpy(info->complete, ptr);
	}
	info->complete_size = ft_strlen(info->complete);
}

int				nothing_to_do(t_ft_info *info)
{
	if (info->tab_ret == NULL)
	{
		ft_strdel(&info->copy);
		return (1);
	}
	return (0);
}

void			do_autocomp(t_ft_info *info, t_line *l, int j, int option)
{
	while (1)
	{
		ft_bzero(l->buffer, 6);
		if (option == 0 && (option = 1))
			l->buffer[0] = 9;
		else
			read(STDIN_FILENO, l->buffer, 6);
		if (cmp_buf((int *)g_actions[0].value, l->buffer) == 0)
		{
			if (info->tab_ret[j] == NULL)
				j = 0;
			if (ft_strcmp(info->tab_ret[j], info->complete) == 0)
				j++;
			if (info->tab_ret[j] != NULL)
			{
				if (info->globing == 1)
					insert_ontop_string(info, l, (info->tab_ret[j]));
				else
					insert_in_string(info, l, (info->tab_ret[j]) + info->complete_size);
				ft_print_line(l);
				j++;
			}
		}
		else
			break;
		if (info->globing == 1 )
			break;
	}
}

void			ft_autocomp(t_line *l)
{
	t_ft_info	info;

	ft_bzero(&info, sizeof(t_ft_info));
	get_prepared(&info, l);

	if (nothing_to_do(&info))
		return ;
	do_autocomp(&info, l, 0, 0);
	del_tab(info.tab_ret);
	ft_strdel(&info.copy);
}
