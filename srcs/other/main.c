/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/13 15:36:17 by jguthert          #+#    #+#             */
/*   Updated: 2017/01/24 21:26:52 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int				g_toto;
int				g_tete;
int				g_titi;
int				g_tata;
int				g_quithist;
int				g_inheredoc_2;
int				g_inheredoc;
t_list			*g_lenv;
t_list			*g_env;
t_ftl_root		g_hist;
t_line			*g_line;
t_line			*g_heredoc;
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
	t_sh sh;

	ft_bzero(&sh.l, sizeof(t_line));
	srand(time(NULL));
	sh.nbr = rand();
	get_history(&g_hist);
	sh.av = NULL;
	handle_var(KV_SET, "?", "0");
	while (1)
	{
		g_toto = g_titi = 1;
		g_tata = 0;
		g_prompt.onshell = 1;
		catch_signal();
		g_prompt.son == 0 ? print_prompt(sh.nbr, g_env, g_lenv, &sh.l) : 0;
		g_prompt = (t_prompt){sh.nbr, g_env, g_lenv, 0, &sh.l, 1};
		g_line = NULL;
		if ((sh.av = read_init(&sh.l, &g_hist)) != NULL)
		{
			g_line = NULL;
			g_prompt.onshell = 0;
			sh.o = shell(sh.av, 0);
			g_prompt.onshell = 1;
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
	g_inheredoc = 0;
	g_quithist = 0;
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
	xmalloc(100);
	if (sh21() == 1)
	{
		hash_del(&(g_hash));
		return (1);
	}
	hash_del(&(g_hash));
	return (0);
}
