/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/19 17:25:51 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/04 21:49:53 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static void		print_env(t_list *env)
{
	while (env != NULL)
	{
		ft_putendl(((t_env *)env->content)->str);
		env = env->next;
	}
}

static int		set_new_arg(char *arg, t_list **g_env, t_list **g_lenv)
{
	char		*my_arg;
	char		*my_cmd;
	int			i;

	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	if (arg[i] != '=')
		return (1);
	my_cmd = ft_strsub(arg, 0, i);
	my_arg = ft_strchr(arg, '=') + 1;
	if (my_cmd == NULL || my_arg == NULL)
		return (1);
	bi_setenv(INIT_AV("setenv", my_cmd, my_arg, 2), g_env, g_lenv);
	ft_strdel(&my_cmd);
	ft_strdel(&my_arg);
	return (0);
}

static int		parse_env(t_av av, t_list **g_env, t_list **g_lenv)
{
	int			i;

	i = -1;
	while (++i < av.argc)
	{
		if (ft_strncmp("-i", av.arg[i], 2) == 0)
			ft_lstdel(g_env, free_env);
		else if (ft_strncmp("-u", av.arg[i], 2) == 0)
		{
			i++;
			bi_unsetenv(INIT_AV(NULL, av.arg[i], NULL, 1), g_env, g_lenv);
		}
		else
			break ;
	}
	while (i < av.argc && set_new_arg(*(av.arg + i), g_env, g_lenv) == 0)
		i++;
	return (i);
}

int				bi_env(t_av av, t_list **g_env, t_list **g_lenv)
{
	int			ret;
	t_av		new_av;

	ret = parse_env(av, g_env, g_lenv);
	new_av.argc = av.argc - ret;
	if (new_av.argc >= 1)
	{
		new_av.cmd = av.arg[ret];
		new_av.arg = av.arg + ret + 1;
		new_av.argv = av.argv + ret + 1;
		check_bin(*g_env, *g_lenv, new_av);//TODO: Gerer env
	}
	else
		print_env(*g_env);
	return (0);
}
