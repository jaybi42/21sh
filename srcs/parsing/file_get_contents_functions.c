/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_get_contents_functions.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/11 16:34:58 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/11 16:35:00 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include "21sh.h"

char	*allocat(int size)
{
	char	*s;

	if ((s = malloc(sizeof(char *) * (size + 1))) == NULL)
		return (NULL);
	s[0] = '\0';
	return (s);
}

char	*binary_cat(char *dest, int len_dest, char *src, int len_src)
{
	int	i;

	i = 0;
	while (i < len_src)
	{
		dest[i + len_dest] = src[i];
		i++;
	}
	return (dest);
}

char	*binary_cpy(char *src, char *dest, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		src[i] = dest[i];
		i++;
	}
	return (src);
}

char	*add_alloc(int fd, int count, t_file *f)
{
	char	*tmp;

	f->str = binary_cat(f->str, f->len_str, f->buffer, f->len_buff);
	f->len_str += f->len_buff;
	if ((tmp = allocat(count * READLEN)) == NULL)
	{
		close(fd);
		return (NULL);
	}
	tmp = binary_cpy(tmp, f->str, f->len_str);
	ft_memdel((void **)(&f->str));
	tmp[f->len_str] = '\0';
	return (tmp);
}

char	*x_allocat(int size)
{
	char	*s;

	if ((s = xmalloc(sizeof(char *) * (size + 1))) == NULL)
		return (NULL);
	s[0] = '\0';
	return (s);
}
