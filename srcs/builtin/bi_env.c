/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/19 17:25:51 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/23 18:48:59 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
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

int				set_new_arg(char *arg, t_list **g_env, t_list **g_lenv)
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
	char		**tmp;

	i = -1;
	while (++i < av.argc)
	{
		if (ft_strncmp("-i", av.arg[i], 2) == 0)
			ft_lstdel(g_env, free_env);
		else if (ft_strncmp("-u", av.arg[i], 2) == 0 && av.arg[i + 1])
		{
			i++;
			bi_unsetenv(INIT_AV(NULL, av.arg[i], NULL, 1), g_env, g_lenv);
		}
		else
			break ;
	}
	while (i < av.argc)
	{
		tmp = x_strsplit(av.arg[i], '=');
		if (!tmp || tlen(tmp) != 2)
			break ;
		bi_setenv(INIT_AV("setenv", tmp[0], tmp[1], 2), g_env, g_lenv);
		i++;
	}
	return (i);
}

void			gen_exec(t_av nv)
{
	int			x;
	size_t		len;
	char		*s;
	t_output	o;

	len = 0;
	s = xmalloc(sizeof(char) * (1));
	s[0] = '\0';
	x = -1;
	while (nv.argv[++x])
	{
		x_strjoins(&s, &len, "\'", 1);
		x_strjoins(&s, &len, nv.argv[x], ft_strlen(nv.argv[x]));
		x_strjoins(&s, &len, "\' ", 2);
	}
	s[len] = '\0';
	(void)o;
	o = shell_exec(s);
	x = -1;
	while (++x < o.len)
		ft_dprintf(2, "%c", o.string[x]);
}

int				bi_env(t_av av, t_list **g_env, t_list **g_lenv)
{
	int			ret;
	t_av		new_av;
	pid_t		pid;

	ft_putendl("toto");
	if ((pid = fork()) == -1)
		return (127);
	else if (pid == 0)
	{
		ret = parse_env(av, g_env, g_lenv);
		new_av.argc = av.argc - ret;
		if (new_av.argc >= 1)
		{
			new_av.cmd = av.arg[ret];
			new_av.arg = av.arg + ret + 1;
			new_av.argv = av.argv + ret + 1;
			gen_exec(new_av);
		}
		else
			print_env(*g_env);
		exit(0);
	}
	else
		wait(NULL);
	return (0);
}
