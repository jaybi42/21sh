/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 16:00:52 by malaine           #+#    #+#             */
/*   Updated: 2016/06/06 19:05:37 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"

int		check_last_word(t_line *l)
{
	int a;

	a = l->count;
	while (l->str[a] != '\0')
	{
		if (l->str[a] == ' ' || l->str[a] == '\t')
			return (0);
		a++;
	}
	return (-1);
}

int		check_first_word(t_line *l)
{
	int a;

	a = l->count;
	while (a >= 0)
	{
		if (l->str[a] == ' ' || l->str[a] == '\t')
			return (0);
		a--;
	}
	return (-1);
}
