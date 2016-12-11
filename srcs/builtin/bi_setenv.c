/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/20 13:28:44 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/11 15:53:40 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int	arg_isin(char *name, char *value, char *str, t_list **g_env)
{
	t_list	*temp;

	temp = *g_env;
	while (temp != NULL)
	{
		if (ft_strcmp(((t_env *)temp->content)->name, name) == 0)
			break ;
		temp = temp->next;
	}
	if (temp == NULL)
		return (1);
	if (name && (ft_strcmp("PATH", name)) == 0)
	{
		hash_del(&g_hash);
		g_hash = hash_table(value);
	}
	if (((t_env *)temp->content)->value != NULL)
		ft_strdel(&((t_env *)temp->content)->value);
	if (((t_env *)temp->content)->str != NULL)
		ft_strdel(&((t_env *)temp->content)->str);
	((t_env *)temp->content)->value = ft_strdup(value);
	if (!((t_env *)temp->content)->value)
		return (0);
	((t_env *)temp->content)->str = str;
	return (0);
}

static int	arg_isout(char *name, char *value, char *str, t_list **g_env)
{
	t_env	env;
	t_list	*new_link;

	if (name)
		if ((ft_strcmp("PATH", name)) == 0)
		{
			hash_del(&g_hash);
			g_hash = hash_table(value);
		}
	env.name = ft_strdup(name);
	env.str = str;
	env.value = ft_strdup(value);
	new_link = ft_lstnew((void *)&env, sizeof(t_env));
	if (env.name == NULL || env.value == NULL || new_link == NULL)
		return (1);
	ft_lstadd_last(g_env, new_link);
	return (0);
}

static int	set_arg(char *name, char *value, t_list **g_env)
{
	char	*temp;
	char	*str;

	temp = ft_strjoin(name, "=");
	if (temp == NULL)
		return (1);
	str = ft_strjoin(temp, value);
	ft_strdel(&temp);
	if (str == NULL)
		return (1);
	if (arg_isin(name, value, str, g_env) == 0)
		return (0);
	else if (arg_isout(name, value, str, g_env) == 0)
		return (0);
	ft_strdel(&str);
	return (1);
}

int			bi_setenv(t_av av, t_list **g_env, t_list **l_env)
{
	int	i;

	i = 0;
	(void)l_env;
	if (av.argc == 0)
		return (bi_env(av, g_env, l_env));
	else if (av.argc > 2)
		return (print_error(av, 0));
	else if (av.argc >= 1 && ft_isalpha(*av.arg[0]) == 0)
		return (print_error(av, 5));
	else if (av.argc >= 1 && ft_strisalnum(*av.arg) == 0)
		return (print_error(av, 4));
	else if (av.argc == 1)
		return (set_arg(*av.arg, "Unknown", g_env));
	else if (av.argc == 2)
		return (set_arg(av.arg[0], av.arg[1], g_env));
	return (0);
}
