/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_del_a_to_b.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/23 08:53:16 by agadhgad          #+#    #+#             */
/*   Updated: 2016/03/09 20:11:56 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_dela_to_b(char *s, int a, int b)
{
	int		i;
	char	*ns;

	i = 1;
	ns = malloc(sizeof(char *) * ft_strlen(s) - (b - a));
	ft_strncpy(ns, s, a);
	a--;
	while (s[b + i])
	{
		ns[a + i] = s[b + i];
		i++;
	}
	ns[a + i] = s[b + i];
	return (ns);
}
