/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 18:44:59 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/17 18:28:38 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PRINT 0
#define INSERT 1
#define DELETE 2
#define DELETEALL 3

#define LEN 10

void			agrandissment(void ***datas, int len)
{
	void	**new_d;
	int		i;

	new_d = malloc(sizeof(void *) * (len + LEN + 1));
	i = 0;
	while (i < len)
	{
		new_d[i] = (*datas)[i];
		i++;
	}
	new_d[i] = NULL;
	free((*datas));
	(*datas) = new_d;
}

int				delete_data(void ***datas, int len, void *to_del)
{
	int i;

	i = -1;
	while (++i < len)
		if ((*datas)[i] == to_del && to_del != NULL)
		{
			//printf("clear a data |%p|\n", to_del);
			if ((*datas)[i] != NULL)
				free((*datas)[i]);
			(*datas)[i] = NULL;
			return (1);
		}
	return (0);
}

void			delete_all(void ***datas, int *len)
{
	int i;

	i = -1;
	while (++i < (*len))
		if ((*datas)[i] != NULL)
			free((*datas)[i]);
	free((*datas));
	if (!((*datas) = malloc(sizeof(void *) * (LEN + 1))))
		return ;
	(*len) = 0;
}

void			print_datas(void **datas, int len)
{
	int i;

	i = -1;
	while (++i < len)
	{
		printf("%d |%p|\n", i, datas[i]);
	}
	printf("garbage length: %d\n", len);
}

void			*storage(int action, void *package)
{
	static void **datas = NULL;
	static int	len = 0;

	if (action == INSERT)
	{
		if (datas == NULL)
			if (!(datas = malloc(sizeof(void *) * (LEN + 1))))
				return (NULL);
		datas[len++] = package;
		datas[len] = NULL;
		if (len % LEN == 0)
			agrandissment(&datas, len);
	}
	else if (action == DELETE)
		delete_data(&datas, len, package);
	else if (action == DELETEALL)
		delete_all(&datas, &len);
	else if (action == PRINT)
		print_datas(datas, len);
	return (NULL);
}

void			*xmalloc(size_t size)
{
	void *al;

	if ((al = malloc(size)) == NULL)
		return (NULL);
	storage(INSERT, al);
	return (al);
}

void			xmemdel(void **p)
{
	storage(DELETE, (*p));
	(*p) = NULL;
}

/*
**	delete all the mem created in a safe way!
*/

void			xmasterfree(void)
{
	storage(DELETEALL, NULL);
}

/*
**	print the mem allocated
*/

void			xprintmem(void)
{
	storage(PRINT, NULL);
}
