/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_alias.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/19 16:21:54 by ibouchla          #+#    #+#             */
/*   Updated: 2016/11/24 20:19:21 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		print_key(t_alias *alias, char *key)
{
	while (alias != NULL)
	{
		if ((ft_strcmp(alias->key, key)) == 0)
		{
			ft_putstr("alias ");
			if (alias->key)
				ft_putstr(alias->key);
			ft_putstr("=\'");
			if (alias->value)
				ft_putstr(alias->value);
			ft_putendl("\'");
			return (0);
		}
		alias = alias->next;
	}
	return (1);
}

int		print_alias_list(t_alias *alias, char *key)
{
	if (key != NULL)
		return ((array_key_exists(alias, key)) ? print_key(alias, key) : 1);
	while (alias != NULL && alias->key != NULL)
	{
		ft_putstr("alias ");
		if (alias->key)
			ft_putstr(alias->key);
		ft_putstr("=\'");
		if (alias->value)
			ft_putstr(alias->value);
		ft_putendl("\'");
		alias = alias->next;
	}
	return (0);
}

char	*strdup_param(char *cmd, int param)
{
	int		i;
	char	c;
	char	*ret;

	i = 0;
	c = ((param == 0) ? '=' : '\0');
	if (cmd == '\0')
		return ((char *)NULL);
	while (((param == 0) ? (ft_isalnum(cmd[i])) : cmd[i] != 0) && (cmd[i] != 0))
		++i;
	if (param == 0)
		if ((cmd[i] != '=') && (cmd[i] != '\0'))
			return ((char *)NULL);
	if (!(ret = ft_strnew(i)))
		return ((char *)NULL);
	i = 0;
	while (((param == 0) ? (ft_isalnum(cmd[i])) : cmd[i] != 0) && (cmd[i] != 0))
	{
		ret[i] = cmd[i];
		++i;
	}
	ret[i] = '\0';
	return (ret);
}

char	**get_data(char *cmd)
{
	int		i;
	int		err;
	char	**pair;

	i = 0;
	err = 0;
	if (!(pair = (char **)ft_memalloc(sizeof(char *) * 2)))
		return ((char **)NULL);
	while ((ft_isspace(cmd[i])) && cmd[i])
		++i;
	pair[0] = strdup_param((cmd + i), 0);
	if (!(ft_isalnum(cmd[i])))
		err = 1;
	while ((ft_isalnum(cmd[i])) && cmd[i] != '\0')
		++i;
	if (cmd[i] != '=')
		err = 1;
	if (err == 0)
	{
		i = ((cmd[i] == '=') ? i + 1 : i);
		pair[1] = strdup_param((cmd + i), 1);
	}
	return (pair);
}

int		bi_alias(t_av av, t_list **g_env, t_list **g_lenv)
{
	int		x;
	char	**pair;

	(void)g_env;
	(void)g_lenv;
	x = (-1);
	if (av.argc == 0)
		return (print_alias_list(g_alias, (char *)NULL));
	while (av.arg[++x] != NULL)
	{
		if ((pair = get_data(av.arg[x])))
		{
			if (pair[0] == NULL)
				ft_putendl_fd("21sh: invalid alias name.", 2);
			else if (pair[1] == NULL)
			{
				if ((print_alias_list(g_alias, pair[0])) != 0)
					ft_putendl_fd("21sh: alias not found.", 2);
			}
			else
				create_or_update_key(&g_alias, pair);
			del_pair(&pair);
		}
	}
	return (0);
}
