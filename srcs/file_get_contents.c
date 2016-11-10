/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_get_contents.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 14:23:42 by agadhgad          #+#    #+#             */
/*   Updated: 2016/11/10 02:45:52 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

#define READLEN 4096

void			ft_memdel(void **ap)
{
	if (ap != NULL)
	{
		free(*ap);
		*ap = NULL;
	}
}

static char		*allocat(int size)
{
	char *s;

	if ((s = malloc(sizeof(char *) * (size + 1))) == NULL)
		return (NULL);
	s[0] = '\0';
	return (s);
}

char			*binary_cat(char *dest, int len_dest, char *src, int len_src)
{
	int i;

	i = 0;
	while (i < len_src)
	{
		dest[i + len_dest] = src[i];
		i++;
	}
	return (dest);
}

char			*binary_cpy(char *src, char *dest, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		src[i] = dest[i];
		i++;
	}
	return (src);
}

typedef struct		s_file
{
	int		len_str;
	int		len_buff;
	char	*str;
	char	buffer[READLEN + 1];
}					t_file;

static char		*add_alloc(int fd, int count, t_file *f)
{
	char *tmp;

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

char			*file_get_contents(char *filename)
{
	int		fd;
	t_file	f;
	int		i;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (NULL);
	if ((f.str = allocat(READLEN)) == NULL)
	{
		close(fd);
		return (NULL);
	}
	f.len_str = 0;
	i = 1;
	while ((f.len_buff = read(fd, f.buffer, READLEN)) > 0)
	{
		i++;
		if ((f.str = add_alloc(fd, i, &f)) == NULL)
			return (NULL);
	}
	close(fd);
	return (f.str);
}

int				file_get_binary(char *filename, char **str, int *len)
{
	int		fd;
	t_file	f;
	int		i;

	(*str) = NULL;
	(*len) = 0;
	if ((fd = open(filename, O_RDONLY)) == -1)
		return (0);
	if ((f.str = allocat(READLEN)) == NULL)
	{
		close(fd);
		return (0);
	}
	f.len_str = 0;
	i = 1;
	while ((f.len_buff = read(fd, f.buffer, READLEN)) > 0)
	{
		i++;
		if ((f.str = add_alloc(fd, i, &f)) == NULL)
			return (0);
	}
	close(fd);
	(*str) = f.str;
	(*len) = f.len_str;
	return (1);
}

int				fd_get_binary(int fd, char **str, int *len)
{
	t_file	f;
	int		i;

	(*str) = NULL;
	(*len) = 0;
	if ((f.str = allocat(READLEN)) == NULL)
	{
		close(fd);
		return (0);
	}
	f.len_str = 0;
	i = 1;
	while ((f.len_buff = read(fd, f.buffer, READLEN)) > 0)
	{
		i++;
		if ((f.str = add_alloc(fd, i, &f)) == NULL)
			return (0);
	}
	close(fd);
	(*str) = f.str;
	(*len) = f.len_str;
	return (1);
}
