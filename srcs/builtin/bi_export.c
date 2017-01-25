/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/25 18:56:55 by jguthert          #+#    #+#             */
/*   Updated: 2017/01/09 17:30:32 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <shell.h>

int		check_option(char *s)
{
	int	i;

	i = 1;
	if (s != NULL && s[0] == '-')
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

int		bi_export(t_av av, t_list **g_env, t_list **g_lenv)
{
	int		p_option;
	int		x;
	char	*str;

	(void)g_env;
	p_option = check_option(av.arg[0]);
	x = ((p_option == 0) ? (-1) : 0);
	if (av.arg[0] == NULL)
		print_all_exported_variables(*g_lenv);
	while (p_option == 0 && av.arg[++x] != NULL)
	{
		str = ft_strtrim(av.arg[x]);
		if ((check_syntaxe(str)) == 0)
			storage_env(g_lenv, str);
		else
			print_bad_syntaxe(str);
		ft_strdel(&str);
	}
	if (p_option == 1)
		option_p(av, g_lenv);
	return (0);
}
