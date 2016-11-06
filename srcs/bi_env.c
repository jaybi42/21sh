/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/19 17:25:51 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/06 19:19:11 by jguthert         ###   ########.fr       */
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

static int		set_new_arg(char *arg, t_list **g_env, t_list **l_env)
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
	my_arg = ft_strchr(arg, '=') + 1;//TODO: TO FREE
	bi_setenv(INIT_AV("setenv", my_cmd, my_arg, 2), g_env, l_env);
	if (my_cmd != NULL)
		ft_strdel(&my_cmd);
	return (0);
}

static int		parse_env(t_av av, t_list **g_env, t_list **l_env)
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
			bi_unsetenv(INIT_AV(NULL, av.arg[i], NULL, 1), g_env, l_env);
		}
		else
			break ;
	}
	while (i < av.argc && set_new_arg(*(av.arg + i), g_env, l_env) == 0)
		i++;
	return (i);
}

int				bi_env(t_av av, t_list **g_env, t_list **l_env)
{
	int			ret;
	t_av		new_av;

	ret = parse_env(av, g_env, l_env);
	new_av.argc = av.argc - ret;
	if (new_av.argc >= 1)
	{
		new_av.cmd = av.arg[ret];
		new_av.arg = av.arg + ret + 1;
		new_av.argv = av.argv + ret + 1;
		check_bin(*g_env, *l_env, new_av);
	}
	else
		print_env(*g_env);
	return (0);
}
