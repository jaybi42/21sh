/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:22:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/25 14:36:20 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include "autocomp.h"
#include <stdio.h> // <<<

static t_actions const  g_actions[1] = {
	{NULL, {9, 0, 0, 0, 0, 0}, "TAB"},
};

static void		copy_tab(char *t, char *data, int *j, int *k)
{
	while (data[*j] != '\0')
	{
		t[*k] = data[*j];
		*j = *j + 1;
		*k = *k + 1;
	}
}

void			insert_in_string(char **str, int i, char *data)
{
	char		*tmp;
	char		*tmp2;
	int			k;
	int			j;

	k = ft_strlen(*str) + ft_strlen(data) + 1;
	if (!(tmp = (char *)malloc(sizeof(char) * k)))
		return ;
	j = 0;
	k = 0;
	tmp2 = ft_strdup(*str);
	while (k < i)
	{
		tmp[k] = tmp2[k];
		k++;
	}
	copy_tab(tmp, data, &j, &k);
	copy_tab(tmp, tmp2, &i, &k);
	tmp[k] = '\0';
	free(*str);
	*str = NULL;
	free(tmp2);
	tmp2 = NULL;
	*str = tmp;
}


void			insert_ontop_string(char **str, int i, char *data)
{
	char		*tmp;
	char		*tmp2;
	int			k;
	int			j;
	int			m;

	m = i;
	while ((*str)[m] != '\0' && (*str)[m] != ' ')
		m++;
	m = m - i;
	k = ft_strlen(*str) + ft_strlen(data) + 1 - m;
	if (!(tmp = (char *)malloc(sizeof(char) * k)))
		return ;
	j = 0;
	k = 0;
	ft_bzero(&tmp, k);
	//printf(" ->( %s )<- ", *str);
	//printf(" ->( %s )<- ", data);
	tmp2 = ft_strdup(*str);
	while (k < i)
	{
		tmp[k] = tmp2[k];
		k++;
	}
	i += m;
	copy_tab(tmp, data, &j, &k);
	copy_tab(tmp, tmp2, &i, &k);
	tmp[k] = '\0';

	free(*str);
	*str = NULL;
	free(tmp2);
	tmp2 = NULL;
	*str = tmp;
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


typedef struct s_tab_info
{
	//1024 bug to fix
	char		complete[1024];
	char		aux[1024];
	char		**t;
	int			function;
	int			index;
	char		*copy;
	int			k;
	int			globing;
}				t_tab_info;

void			get_prepared(t_tab_info *info, t_line *l)
{
	char	*ptr;

	info->copy = ft_strdup(l->str);
	info->index = l->count;
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
	if (!info->function)
		info->t = ret_match(info->complete);
	else
		info->t = command_fnc(info->complete);

	ft_strcpy(info->aux, info->complete);
	ptr = ft_strrchr(info->complete, '/');
	if (ptr != NULL)
	{
		ptr++;
		ft_strcpy(info->complete, ptr);
	}
}

int				nothing_to_do(t_tab_info *info)
{
	if (info->t == NULL)
	{
		free(info->copy);
		return (1);
	}
	return (0);
}

void			ft_autocomp(t_line *l)
{
	t_tab_info	info;

	ft_bzero(&info, sizeof(t_tab_info));
	get_prepared(&info, l);

	if (nothing_to_do(&info))
		return ;

	char	buff[MAX_CANON];
	int 	j = 0;
	char	*tmp2;
	int		test = 0;
	int		test1 = ft_strlen(info.complete);
	int		test2 = ft_strlen(info.aux);
	int		stop;

	stop = 1;
		tmp2 = NULL;
	while (stop)
	{
		tmp2 = ft_strdup(info.copy);
		ft_bzero(&buff, MAX_CANON);
		if (test == 0 && (test = 1))
			buff[0] = 9;
		else
			read(STDIN_FILENO, buff, MAX_CANON);
		if (cmp_buf((int *)g_actions[0].value, buff) == 0)
		{
			if (info.t[j] == NULL)
				j = 0;
			if (info.t[j] != NULL)
			{
				if (info.globing == 1)
					insert_ontop_string(&tmp2, info.index - test2, (info.t[j]));
				else
					insert_in_string(&tmp2, info.index, (info.t[j]) + test1);
				free(l->str);
				l->str = ft_strdup(tmp2);
				l->size = ft_strlen(l->str);
				if (info.globing == 1)
					l->count = (info.index - test2) + ft_strlen(info.t[j]);
				else
					l->count = info.index + ft_strlen((info.t[j]) + test1);
				ft_print_line(l);
				j++;
			}
		}
		else
			stop = 0;
		if (info.globing == 1)
			stop = 0;
		if (tmp2 != NULL)
		{
			free(tmp2);
			tmp2 = NULL;
		}
	}
	del_tab(info.t);
	free(info.copy);
}
