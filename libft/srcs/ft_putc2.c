/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/01 18:36:15 by agadhgad          #+#    #+#             */
/*   Updated: 2017/01/06 19:38:47 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*exchange(char **s, size_t l)
{
	char *snew;

	snew = malloc(sizeof(char *) * l);
	snew[0] = '\0';
	ft_strcpy(snew, *s);
	ft_strdel(s);
	return (snew);
}

int				get_fd(int fd)
{
	static int fd_s = 1;

	if (fd != 0)
		fd_s = fd;
	return (fd_s);
}

void			writex2(size_t *l, size_t *i, char **s)
{
	(*l) = 100;
	(*s) = malloc(sizeof(char) * (*l));
	(*i) = 0;
}

void			writex(char *str, size_t length, int print)
{
	static char		*s;
	static size_t	l;
	static size_t	i;

	if (print == -1)
		writex2(&l, &i, &s);
	else if (print == 0)
	{
		i += length;
		if (i < l)
		{
			l += 100;
			s = exchange(&s, l);
			i = 0;
		}
		if (length <= 1)
			ft_strncat(s, str, length);
		else
			ft_strcat(s, str);
	}
	else if (print == 1)
	{
		write(get_fd(0), s, ft_strlen(s));
		ft_strdel(&s);
	}
}
