/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/25 18:56:55 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/25 18:56:56 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <21sh.h>

int		check_option(char *s)
{
	int	i;

	i = 1;
	if (s != NULL)
	{
		if (s[0] == '-')
		{
			while (s[i] == 'p' && s[i] != '\0')
				++i;
			if (s[i] == '\0' && i != 1)
				return (1);
			if (s[i] != '\0')
			{
				ft_putstr_fd("42sh: export: -", 2);
				ft_putchar_fd(s[i], 2);
				ft_putstr_fd(": invalid option\n", 2);
				ft_putstr_fd("export: usage: export [-p] [name[=value] ...]\n", 2);
				return (-1);
			}
		}
	}
	return (0);
}

int		check_syntaxe(char *s)
{
	int	i;

	i = 0;
	if (s[0] == '=')
		return (1);
	while ((ft_isalpha(s[i])) && s[i] != '=' && s[i] != '\0')
		++i;
	return ((s[i] == '\0' || s[i] == '=') ? 0 : 1);
}

void	print_bad_syntaxe(char *s)
{
	ft_putstr_fd("export: \'", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\': not a valid identifier\n", 2);
}

void	print_all_exported_variables(t_list *env)
{
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

int		bi_export(t_av av, t_list **g_env, t_list **l_env)
{
	int		p_option;
	int		x;
	char	*str;

	(void)l_env;
	p_option = check_option(av.arg[0]);
	x = ((p_option == 0) ? (-1) : 0);
	while (av.arg[++x] != NULL)
	{
		str = ft_strtrim(av.arg[x]);
		if ((check_syntaxe(str)) == 0)
			storage_env(g_env, str);
		else
			print_bad_syntaxe(str);
		ft_strdel(&str);
	}
	if (p_option == 1)
		print_all_exported_variables(*g_env);
	return (0);
}
