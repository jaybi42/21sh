/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:22:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/21 19:11:34 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "autocomp.h"

static t_actions const g_actions[2] = {
	{NULL, {9, 0, 0, 0, 0, 0}, "TAB"},
	{NULL, {9, 0, 0, 0, 0, 0}, "TAB"},
};

static void			get_prepared(t_ft_info *info, t_line *l)
{
	char	*ptr;
	char	*ptr2;

	init(info, l);
	if (!info->function)
		info->tab_ret = ret_match(info->complete);
	else
		info->tab_ret = command_fnc(info->complete);
	if ((ptr = ft_strrchr(info->complete, '/')) != NULL)
	{
		*ptr = '\0';
		ptr++;
		ptr2 = ft_strdup(ptr);
		ft_bzero(info->complete, info->size);
		ft_strcpy(info->complete, ptr2);
		ft_strdel(&ptr2);
	}
	info->complete_size = ft_strlen(info->complete);
}

static int			nothing_to_do(t_ft_info *info)
{
	if (info->tab_ret == NULL)
	{
		del_tab(info->tab_ret);
		ft_strdel(&info->copy);
		ft_strdel(&info->aux);
		ft_strdel(&info->complete);
		ft_strdel(&info->copy2);
		return (1);
	}
	return (0);
}

static void			my_read(t_line *l, int *option)
{
	ft_bzero(l->buffer, 6);
	if (*option == 0 && (*option = 1))
		l->buffer[0] = 9;
	else
		read(STDIN_FILENO, l->buffer, 6);
}

static void			do_autocomp(t_ft_info *info, t_line *l, int j, int *option)
{
	while (1)
	{
		my_read(l, option);
		if (cmp_buf((int *)g_actions[0].value, l->buffer) == 0)
		{
			j = info->tab_ret[j] == NULL ? 0 : j;
			if (info->tab_ret[j] != NULL)
			{
				if (info->globing == 1)
					insert_ontop_string(info, l, (info->tab_ret[j]));
				else
					insert_in_string(info, l,
							(info->tab_ret[j]) + info->complete_size);
				ft_swap_print(l);
				j++;
			}
		}
		else
		{
			actions(l);
			break ;
		}
		if (info->globing == 1)
			break ;
	}
}

void				ft_autocomp(t_line *l)
{
	t_ft_info	info;
	int			option;

	option = 0;
	ft_bzero(&info, sizeof(t_ft_info));
	get_prepared(&info, l);
	if (nothing_to_do(&info))
		return ;
	do_autocomp(&info, l, 0, &option);
	del_tab(info.tab_ret);
	ft_strdel(&info.copy);
	ft_strdel(&info.aux);
	ft_strdel(&info.complete);
	ft_strdel(&info.copy2);
}
