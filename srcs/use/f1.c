/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f1.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 20:09:19 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/06 20:13:14 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	**fstrsplit(char *str, int len, int (*is_whatever)(char))
{
	char	**t_str;
	int		i;
	int		a;
	int		beg;

	if (!(t_str = xmalloc(sizeof(char **) * (len + 1))))
		return (NULL);
	t_str[0] = NULL;
	i = 0;
	a = 0;
	beg = 0;
	while (i < len && str[i] != '\0')
	{
		while (i < len && is_whatever(str[i]) && str[i++] != '\0')
			;
		beg = i;
		while (i < len && !is_whatever(str[i]) && str[i++] != '\0')
			;
		if (beg != i)
			t_str[a++] = cpy_a_to_b(str, beg, i);
	}
	t_str[a] = NULL;
	return (t_str);
}

char	**join_array(char ***t)
{
	char	**ts;
	int		a;
	int		i;
	int		l;

	a = -1;
	l = 0;
	while (t[++a])
	{
		i = -1;
		while (t[a][++i])
			l++;
	}
	ts = xmalloc(sizeof(char *) * (l + 1));
	a = -1;
	l = 0;
	while (t[++a])
	{
		i = -1;
		while (t[a][++i])
			ts[l++] = t[a][i];
	}
	ts[l] = NULL;
	return (ts);
}

char	**copy_array_begin(size_t b, char **array)
{
	int		i;
	char	**new_array;

	if (array == NULL)
		return (NULL);
	i = 0;
	while (array[i] != NULL)
		i++;
	if (!(new_array = xmalloc(sizeof(char **) * (i + 1))))
		return (NULL);
	if (i == 0)
	{
		new_array[i] = NULL;
		return (new_array);
	}
	i = 0;
	while (array[i + b] != NULL)
	{
		new_array[i] = array[i + b];
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}
