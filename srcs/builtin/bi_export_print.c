/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/09 17:05:06 by jguthert          #+#    #+#             */
/*   Updated: 2017/01/24 20:51:06 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void		print_export_err(char *str)
{
	ft_putstr("export: no such variable: ");
	ft_putendl(str);
}

void			print_all_exported_variables(t_list *env)
{
	ft_lst_sort(&env, &ft_strcmp);
	while (env != NULL)
	{
		ft_putstr("export ");
		if (((t_env *)env->content)->name != NULL)
			ft_putstr(((t_env *)env->content)->name);
		ft_putstr("=\"");
		if (((t_env *)env->content)->value != NULL)
			ft_putstr(((t_env *)env->content)->value);
		ft_putendl("\"");
		env = env->next;
	}
}

static int		ft_getenv(char *str, t_list **g_env)
{
	t_list		*env;

	env = *g_env;
	while (env != NULL)
	{
		if (ft_strcmp(str, ((t_env *)env->content)->name) == 0)
		{
			ft_putstr("export ");
			ft_putstr(str);
			ft_putstr("='");
			ft_putstr(((t_env *)env->content)->value);
			ft_putendl("'");
			return (0);
		}
		env = env->next;
	}
	return (1);
}

void			option_p(t_av av, t_list **g_env)
{
	int i;

	i = 1;
	while (av.arg[i] != NULL)
	{
		if (ft_getenv(av.arg[i], g_env) == 1)
			print_export_err(av.arg[i]);
		i++;
	}
	if (i == 1)
		print_all_exported_variables(*g_env);
}
