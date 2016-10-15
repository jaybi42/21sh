/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/07 16:22:22 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/15 19:41:18 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include "autocomp.h"
#include <stdio.h> // <<<

static t_actions const  g_actions[1] = {
	{NULL, {9, 0, 0, 0, 0, 0}, "TAB"},
};

static void		copy_tab(char *tab, char *data, int *j, int *k)
{
	while (data[*j] != '\0')
	{
		tab[*k] = data[*j];
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
	free(tmp2);
	*str = tmp;
}

int				test_if_fnc(int i, char *str)
{
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
		else if (str[i - 1] == '|')
			return (1);
	}
	return (0);
}



void			ft_autocomp(t_line *l)
{
	char 	*tmp;
	int		i;
	char 	complete[1024];
	char	**tab;
	int 	k = 0;
	int		function;

	function = 0;
	tmp = ft_strdup(l->str);
	i = l->count;
	while (i > 0 && tmp[i - 1] != ' ')
		i--;
	function = test_if_fnc(i, tmp);
	while (tmp[i] != ' ' && tmp[i] != '\0')
	{
		complete[k] = tmp[i];
		i++;
		k++;
	}

	complete[k] = '\0';
	if (!function)
		tab = ret_match(complete);
	else
		tab = command_fnc(complete);	
	
	char *ptr;
	ptr = ft_strrchr(complete, '/');

	if (ptr != NULL)
	{
		ptr++;
		ft_strcpy(complete, ptr);
	}
	k = 0;
	if (tab == NULL)
		return ;

	char	buff[MAX_CANON];
	int 	j = 0;
	char	*tmp2;
	int		test = 0;
	int		test1 = ft_strlen(complete);
	int		stop;

	stop = 1;

	while (stop)
	{
		tmp2 = ft_strdup(tmp);
		(void)ft_memset((void *)buff, 0, MAX_CANON);
		if (test == 0 && (test = 1))
			buff[0] = 9;
		else
			(void)read(STDIN_FILENO, buff, MAX_CANON);
		if (cmp_buf((int *)g_actions[0].value, buff) == 0)
		{
			if (tab[j] == NULL)
				j = 0;
			if (complete[0] != '.')
			{
				while (tab[j] != NULL && (tab[j][0] == '.'))
					j++;
			}
			else if (complete[0] == '.')
			{
				while (ft_strcmp(tab[j], ".") == 0 || ft_strcmp(tab[j], "..") == 0)
					j++;
			}
			if (tab[j] != NULL)
			{
				insert_in_string(&tmp2, i, (tab[j]) + test1);
				free(l->str);
				l->str = ft_strdup(tmp2);
				l->size = ft_strlen(l->str);
				l->count = i + ft_strlen((tab[j]) + test1);
				ft_print_line(l);
				j++;
			}
			if (tmp2 != NULL)
				free(tmp2);
		}
		else
			stop = 0;
	}
	del_tab(tab);
	free(tmp);
}
