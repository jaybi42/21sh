/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globuse2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 16:18:11 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/06 18:56:36 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	update_t(char ***p_t, int **p_i_a, int *p_ta, char c)
{
	int i;

	i = -1;
	while (++i < (*p_ta))
		(*p_t)[i][(*p_i_a)[i]++] = c;
}

char	*cpy_a_to_b2(char *str, int a, int b)
{
	char	*new_str;
	int		len;
	int		i;

	len = (b - a < 0) ? 0 : b - a;
	if (!(new_str = xmalloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	while (a + i < b)
	{
		new_str[i] = str[a + i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

int		tlen(char **t)
{
	int	i;

	i = -1;
	while (t[++i])
		;
	return (i);
}

int		accolade_isit(char c)
{
	if (c == ',')
		return (1);
	else
		return (0);
}
