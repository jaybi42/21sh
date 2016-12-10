/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storage_new_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/25 18:56:29 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/10 15:57:50 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <21sh.h>

char		*isolate(char *new_var, int obj)
{
	int	i;
	char	*elem;

	i = (-1);
	if (obj == 0)
	{
		while (new_var[++i] != '=' && new_var[i] != '\0')
			;
		elem = ((new_var[i] == '=' || new_var[i] == '\0')
				? ft_strsub(new_var, 0, i) : NULL);
	}
	else
	{
		elem = (((ft_strchr(new_var, '=')) == NULL) ? ft_strchr(new_var, '=') :
				(ft_strchr(new_var, '=') + 1));
		if (elem != NULL)
			if ((ft_strcmp(elem, "=")) == 0)
				elem = NULL;
	}
	return (elem);
}

static int	replace_key_value(t_list **addr, char *new_var, char *value)
{
	char	*temp;

	temp = ft_strdup(new_var);
	if (temp == NULL)
		return (1);
	if (((t_env *)(*addr)->content)->str != NULL)
		ft_strdel(&((t_env *)(*addr)->content)->str);
	((t_env *)(*addr)->content)->str = temp;
	temp = ft_strdup(value);
	if (temp == NULL)
		return (1);
	if (((t_env *)(*addr)->content)->value != NULL)
		ft_strdel(&((t_env *)(*addr)->content)->value);
	((t_env *)(*addr)->content)->value = temp;
	return (0);
}

int			if_key_replace(char *new_var, t_list **g_env)
{
	t_list	*temp;
	char	*name;
	char	*value;

	temp = *g_env;
	name = isolate(new_var, 0);
	if (name == NULL)
		return (1);
	value = isolate(new_var, 1);
	while (temp != NULL)
	{
		if ((ft_strcmp(((t_env *)temp->content)->name, name)) == 0)
			break ;
		temp = temp->next;
	}
	ft_strdel(&name);
	if (temp == NULL || value == NULL)
		return (1);
	return (replace_key_value(&temp, new_var, value));
}

void		storage_env(t_list **e, char *new_var)
{
	int		i;
	t_env	env;
	t_list	*new_node;

	i = 0;
	if ((if_key_replace(new_var, e)) == 0)
		return ;
	env.str = ft_strdup(new_var);
	while (new_var[i] != '=' && new_var[i] != '\0')
		++i;
	env.name = ft_strsub(new_var, 0, i);
	if (new_var[i] != '\0' && new_var[++i] != '\0')
		env.value = ft_strsub(new_var, i, ft_strlen(new_var) - i);
	else
		env.value = NULL;
	new_node = ft_lstnew((void *)&env, sizeof(env));
	if (new_node)
		ft_lstadd_last(e, new_node);
}
