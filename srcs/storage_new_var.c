
#include <21sh.h>

char	*isolate(char *new_var, int obj)
{
	int		i;
	char	*elem;

	i = (-1);
	if (obj == 0)
	{
		while(new_var[++i] != '=' && new_var[i] != '\0')
			;
		elem = ((new_var[i] == '=' || new_var[i] == '\0') ? ft_strsub(new_var, 0, i) : NULL);
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

int		check_if_key_exist(char *new_var, t_list **g_env)
{
	t_list	*temp;
	char	*name;
	char	*value;

	temp = *g_env;
	if ((name = isolate(new_var, 0)) == NULL)
		return (1);
	value = isolate(new_var, 1);
	while (temp != NULL)
	{
		if ((ft_strcmp(((t_env *)temp->content)->name, name)) == 0)
			break ;
		temp = temp->next;
	}
	ft_strdel(&name);
	if (temp == NULL)
		return (1);
	if (value == 0)
		return (0);	
	if (((t_env *)temp->content)->value != NULL)
		ft_strdel(&((t_env *)temp->content)->value);
	if (((t_env *)temp->content)->str != NULL)
		ft_strdel(&((t_env *)temp->content)->str);
	((t_env *)temp->content)->value = ft_strdup(value);
	((t_env *)temp->content)->str = ft_strdup(new_var);
	return (0);
}

void	storage_env(t_list **e, char *new_var)
{
	int		i;
	t_env	env;
	t_list	*new_node;

	i = 0;
	if ((check_if_key_exist(new_var, e)) == 0)
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
