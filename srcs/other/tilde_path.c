/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 18:11:00 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/10 16:27:51 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

char	*create_str(char *str, char *home)
{
	size_t	i;
	size_t	j;
	char	*ret;

	i = 0;
	j = 0;
	while (((ft_isprint(str[i])) &&
	!(char_is_whitespace(str[i]))) && str[i] != '\0')
		++i;
	if ((ret = x_strnew(ft_strlen(home) + i)) == NULL)
		return (NULL);
	i = (-1);
	while (home[++i] != '\0')
		ret[i] = home[i];
	while (((ft_isprint(str[++j])) &&
	!(char_is_whitespace(str[j]))) && str[j] != '\0')
		ret[i++] = str[j];
	ret[i] = '\0';
	return (ret);
}

char	*tilde_path(char *str, char *home)
{
	int	i;

	i = 0;
	while ((char_is_whitespace(str[i])) && str[i] != '\0')
		++i;
	if (str[i] != '~')
		return (NULL);
	return (((home == NULL) ? NULL : create_str((str + i), home)));
}
