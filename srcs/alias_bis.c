
#include "21sh.h"

void	create_new_node(t_alias **addr, char **pair)
{
	t_alias	*new_node;
	t_alias	*alias;
	t_alias	*tmp;

	if (!(new_node = (t_alias *)ft_memalloc(sizeof(t_alias))))
		return ;
	new_node->key = ((pair[0] != NULL) ? ft_strdup(pair[0]) : NULL);
	new_node->value = ((pair[1] != NULL) ? ft_strdup(pair[1]) : NULL);
	alias = *addr;
	tmp = NULL;
	if (pair[0] != NULL)
		while ((alias != NULL) && ((ft_strcmp(alias->key, pair[0])) > 0))
		{
			tmp = alias;
			alias = alias->next;
		}
	new_node->next = alias;
	if (tmp != NULL)
		tmp->next = new_node;
	else
		*addr = new_node;
}

void	create_or_update_key(t_alias **addr, char **pair)
{
	int		id;
	t_alias	*alias;
	int i;

	i = 0;
	alias = *addr;
	if ((id = array_key_exists(alias, pair[0])))
	{
		while ((i < id) && (alias->next != NULL))
			alias = alias->next;
		if (alias != NULL)
			if (pair[1] != NULL)
			{
				ft_strdel(&(alias->value));
				alias->value = ft_strdup(pair[1]);
			}
	}
	else
		create_new_node(addr, pair);
}

void	del_pair(char ***pair)
{
	if ((*pair)[0] != NULL)
		ft_strdel(&((*pair)[0]));
	if ((*pair)[1] != NULL)
		ft_strdel(&((*pair)[1]));
	if (*pair != NULL)
		ft_memdel((void **)&(*pair));
}

int		array_key_exists(t_alias *alias, char *key)
{
	int	i;

	i = 1;
	while (alias != NULL)
	{
		if (key != NULL)
			if ((ft_strcmp(alias->key, key)) == 0)
				return (i);
		alias = alias->next;
		++i;
	}
	return (0);
}
