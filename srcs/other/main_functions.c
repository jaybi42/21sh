/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/11 16:00:35 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/11 16:00:38 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int		ret_exit(int state, int value)
{
	static int	data;

	data = 0;
	if (state == SET)
	{
		data = value;
	}
	return (data);
}

void	clean_exit(int ret)
{
	put_history(&g_hist);
	a_stop(0);
	exit(ret);
}

int		insert_arr(char ***a, char *s)
{
	int		i;
	char	**na;

	i = 0;
	if (!a || !(*a) || !s)
		return (-1);
	while ((*a)[i++])
		;
	if (!(na = malloc(sizeof(char *) * (i + 2))))
		clean_exit(8);
	i = 0;
	while ((*a)[i])
	{
		na[i] = (*a)[i];
		i++;
	}
	na[i] = ft_strdup(s);
	na[i + 1] = NULL;
	ft_memdel((void **)a);
	(*a) = na;
	return (0);
}

int		search_key_values(char **keys, char **values, char *key)
{
	int i;

	i = 0;
	if (!key || !keys || !values)
		return (-1);
	while (keys[i])
	{
		if (values[i] == NULL)
			return (-1);
		if (ft_strcmp(keys[i], key) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*delete_key(char ***keys, char ***values, char *key)
{
	int i;
	int success;
	int a;

	i = 0;
	a = 0;
	success = FALSE;
	while ((*keys)[i])
	{
		if (!(*values)[i])
			return (NULL);
		if (ft_strcmp((*keys)[i], key) != 0)
		{
			(*values)[a] = (*values)[i];
			(*keys)[a++] = (*keys)[i];
		}
		else
			success = TRUE;
		i++;
	}
	if (success)
		return (key);
	return (NULL);
}
