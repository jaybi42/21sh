/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/13 15:36:17 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/06 19:07:27 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>
#include <time.h>

t_list		*l_env;
t_list		*g_env;
t_ftl_root	g_hist;
t_line		*g_line;

int				ret_exit(int state, int value)
{
	static int	data = 0;

	if (state == SET)
	{
		data = value;
	}
	return (data);
}



void		clean_exit(int ret)
{
	put_history(&g_hist);
	a_stop(0);
	exit(ret);
}


int			insert_arr(char ***a, char *s)
{
	int		i;
	char	**na;

	i = 0;
	if (!a || !(*a) || !s)
		return (-1);
	while((*a)[i++]);
	if (!(na = malloc(sizeof(char *) * (i + 2))))
		clean_exit(8);
	i = 0;
	while ((*a)[i])
	{
		na[i] = (*a)[i];
		i++;
	}
	na[i] = s;
	na[i + 1] = NULL;
	ft_memdel((void **)a);
	(*a) = na;
	return (0);
}

int			search_key_values(char **keys, char **values, char *key)
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

char		*delete_key(char ***keys, char ***values, char *key)
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

char		*handle_var(int state, char *key, char *value)
{
	static char **keys = NULL;
	static char **values = NULL;
	int tmp;

	if (!keys && !values)
	{
		if (!(keys = malloc(sizeof(char*))))
			clean_exit(8);
		keys[0] = NULL;
		if (!(values = malloc(sizeof(char*))))
			clean_exit(8);
		values[0] = NULL;
	}

	/*a_printf("test handle_var: |%s| and |%s|\n", key, value);
	for (int i = 0; keys[i]; i++)
	{
		a_printf("|%s| => |%s| \n", keys[i], values[i]);
	}*/
	if (state == KV_SET)
	{
		if (key == NULL || value == NULL)
			return (NULL);
		if((tmp = search_key_values(keys, values, key)) != -1)
		{
			keys[tmp] = ft_strdup(key);
			values[tmp] = ft_strdup(value);
			return (key);
		}
		if (insert_arr(&keys, key) == -1 ||
			insert_arr(&values, value) == -1)
			return (NULL);
		return (key);
	}
	else if (state == KV_GET)
	{
		if ((tmp = search_key_values(keys, values, key)) == -1)
			return (NULL);
		return (values[tmp]);
	}
	else if (state == KV_DEL)
		return (delete_key(&keys, &values, key));
	return (NULL);
}


void			ft_printmem(t_av **av)
{
	(void)av;
}


static int		sh21(void)
{
	t_av **av;
	t_line		l;
	uint64_t	nbr;
	t_output o;
	ft_bzero(&l, sizeof(t_line));
	srand(time(NULL));
	nbr = rand();
	get_history(&g_hist);
	av = NULL;
	handle_var(KV_SET, "?", "0");
	while (1)
	{

		catch_signal((t_prompt){nbr, g_env, l_env, 0, &l});
		print_prompt(nbr, g_env, l_env, &l);
		g_line = NULL;
		if ((av = read_init(&l, &g_hist)) == NULL)
		{
			printf("special error\n");
			clean_exit(53);
		}
		g_line = NULL;
		o = shell(av, 0);
		xmasterfree();
	}
	return (1);
}

int				main(void)
{
	g_env = NULL;
	l_env = NULL;
	g_line = NULL;
	if (init_env(&g_env, &l_env) == 1)
		return (1);
	if (a_init() == -1)
		return (1);
	if (sh21() == 1)
		return (1);
	return (0);
}
