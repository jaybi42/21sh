/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/13 15:36:17 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/11 22:54:41 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

t_list			*g_lenv;
t_list			*g_env;
t_ftl_root		g_hist;
t_line			*g_line;
t_prompt		g_prompt;
t_alias			*g_alias;
int				*g_exit;
int				g_debug;
t_hash			**g_hash;
unsigned int	g_hash_size;
char			*g_heredoc_expected;

char			*handle_var_set(char ***keys, char ***values,
		char *key, char *value)
{
	int			tmp;

	if (key == NULL || value == NULL)
		return (NULL);
	if ((tmp = search_key_values((*keys), (*values), key)) != -1)
	{
		ft_strdel(&(*keys)[tmp]);
		ft_strdel(&(*values)[tmp]);
		(*keys)[tmp] = ft_strdup(key);
		(*values)[tmp] = ft_strdup(value);
		return (key);
	}
	if (insert_arr(&(*keys), key) == -1 ||
			insert_arr(&(*values), value) == -1)
		return (NULL);
	return (key);
}

char			*handle_var(int state, char *key, char *value)
{
	static char	**keys = NULL;
	static char	**values = NULL;
	int			tmp;

	if (!keys && !values)
	{
		if (!(keys = malloc(sizeof(char*))))
			clean_exit(8);
		keys[0] = NULL;
		if (!(values = malloc(sizeof(char*))))
			clean_exit(8);
		values[0] = NULL;
	}
	if (state == KV_SET)
		return (handle_var_set(&keys, &values, key, value));
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

int				sh21(void)
{
	t_av		**av;
	t_line		l;
	uint64_t	nbr;
	t_output	o;

	ft_bzero(&l, sizeof(t_line));
	srand(time(NULL));
	nbr = rand();
	get_history(&g_hist);
	av = NULL;
	handle_var(KV_SET, "?", "0");
	while (1)
	{
		catch_signal();
		((g_prompt.son == 0) ? print_prompt(nbr, g_env, g_lenv, &l) : (0));
		g_prompt = (t_prompt){nbr, g_env, g_lenv, 0, &l};
		g_line = NULL;
		if ((av = read_init(&l, &g_hist)) != NULL)
		{
			g_line = NULL;
			o = shell(av, 0);
		}
		xmasterfree();
	}
	return (1);
}

void			init_global(int ac, char **argv)
{
	g_debug = FALSE;
	if (ac >= 2 && ft_strcmp(argv[1], "--debug") == 0)
	{
		g_debug = TRUE;
		ft_dprintf(2, "-- {red}WELCOME ON THE DEBUG MODE{eoc} --\n");
	}
	g_env = NULL;
	g_env = NULL;
	g_lenv = NULL;
	g_line = NULL;
	g_alias = mmap(NULL, sizeof(*g_alias), PROT_READ | PROT_WRITE, MAP_SHARED
			| MAP_ANONYMOUS, -1, 0);
	g_exit = mmap(NULL, sizeof(*g_exit), PROT_READ | PROT_WRITE, MAP_SHARED
			| MAP_ANONYMOUS, -1, 0);
	*g_exit = -1;
	g_hash = NULL;
	g_hash_size = 0;
	g_heredoc_expected = NULL;
}

int				main(int ac, char **argv)
{
	init_global(ac, argv);
	if (init_env(&g_env, &g_lenv) == 1 || (a_init() == -1))
		return (1);
	g_hash = hash_table(get_path(g_env, g_lenv));
	if (sh21() == 1)
	{
		hash_del(&(g_hash));
		return (1);
	}
	hash_del(&(g_hash));
	return (0);
}
