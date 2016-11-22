
#include <21sh.h>

char	*get_alias_elem_by_id(t_alias *alias, int id)
{
	int	i;

	i = 0;
	if (alias != NULL)
	{
		while (++i < id && alias->next != NULL)
			alias = alias->next;
		return (ft_strdup(alias->value));
	}
	return ((char *)NULL);
}

int		already_searched(t_list *key_list, char *new_key)
{
	while (key_list != NULL)
	{
		if ((ft_strcmp((char *)key_list->content, new_key)))
			return (1);
		key_list = key_list->next;
	}
	return (0);
}

char	*get_alias(char **addr_bin)
{
	char	*tmp;
	int		id;
	t_list	*key_list;

	tmp = ft_strdup(*addr_bin);
	while ((id = array_key_exists(g_alias, tmp)))
	{
		ft_lstadd(&key_list, ft_lstnew((void *)tmp, (ft_strlen(tmp) + 1)));	// On ajoute la clef parcouru
		ft_strdel(&tmp);
		tmp = get_alias_elem_by_id(g_alias, id);	// On renvoi la value a la pos x qui deviendra notre nouvelle clef
		if ((already_searched(key_list, tmp)))	// On envoi l'actuel valeure a la fonction qui check notre liste de clef déjà parcouru = loop infinie
		{
			ft_strdel(&tmp);
			ft_lstdel(&key_list, &free_key);
			return (*addr_bin);
		}
	}
	ft_strdel(addr_bin);
	ft_lstdel(&key_list, &free_key);
	return (tmp);
}

/*
**
**	Tant que la valeure d'une clef est présente en tant que clef dans la liste d'alias
**	On continue jusqu'a que une value valle les X clef qu'on a parcouru ou que value == clef
**
**	_______________
**	id  key    val
**
**	[0] {l}  | {Ab}
**	[1] {Ab} | {Ba}
**	[2] {Ba} | {Ac}
**	[3] {Ac} | {XX}
**	[4] {XX} | {ls -lah}
**
*/
