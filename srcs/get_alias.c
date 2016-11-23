/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_alias.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/22 20:53:56 by ibouchla          #+#    #+#             */
/*   Updated: 2016/11/22 22:00:57 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <21sh.h>

char	*get_alias_elem_by_id(t_alias *alias, int id)
{
	int	i;

	i = 0;
	if (alias != NULL)
	{
		while (++i < id && alias->next != NULL)
			alias = alias->next;
		return (((alias->value != NULL) ? ft_strdup(alias->value) : NULL));
	}
	return ((char *)NULL);
}

int		already_searched(t_list *key_list, char *new_key)
{
	while (key_list != NULL)
	{
		if ((ft_strcmp((char *)key_list->content, new_key)) == 0)
			return (1);
		key_list = key_list->next;
	}
	return (0);
}

/*
 **	On va devoir split la derniere value alias obtenu et joindre tout ce qu'on a (a partir de argv[1] pour sauter le binaire)
*/

char	**tabjoin(char **t1, char **t2)
{
	char	**new_tab;
	size_t	i;
	size_t	n;

	i = (-1);
	new_tab = (char **)ft_memalloc(sizeof(char *) * ((ft_size_tab(t1) + ft_size_tab(t2)) + 1));
	while (t1[++i] != NULL)
		new_tab[i] = ft_strdup(t1[i]);
	n = i;
	i = (-1);
	while (t2[++i] != NULL)
		new_tab[n++] = ft_strdup(t1[i]);
	new_tab[n] = NULL;
	return (new_tab);
}

char	**tabdup(char **t)
{
	size_t	i;
	char	**new_tab;

	i = (-1);
	new_tab = (char **)ft_memalloc(sizeof(char *) * (ft_size_tab(t) + 1));
	while (t[++i] != NULL)
		new_tab[i] = ft_strdup(t[i]);
	new_tab[i] = NULL;
	return (new_tab);
}

void	do_all_stuff(t_av **av, char *key_value)
{
	char	**tmp;

	//	ft_tabdel((*av)->argv);	
	//	ft_tabdel((*av)->arg);
	
	//ft_strdel(&((*av)->cmd));
	if (!(tmp = ft_strsplit(key_value, ' ')))
		return ;
	(*av)->cmd = ((key_value != NULL) ? ft_strdup(tmp[0]) : NULL);
	(*av)->argv = ((key_value != NULL) ? tabjoin(tmp, (*av)->arg) : NULL);
	(*av)->arg = ((key_value != NULL) ? tabdup(((*av)->argv + 1)) : NULL);
	//ft_tabdel(tmp);
}

void	get_alias(t_av **av)
{
	char	*tmp;
	int		id;
	t_list	*key_list;

	if (!((*av)->cmd))
		return ;
	tmp = ft_strdup((*av)->cmd);
	while ((id = array_key_exists(g_alias, tmp)))
	{
		ft_lstadd(&key_list, ft_lstnew((void *)tmp, (ft_strlen(tmp) + 1)));	// On ajoute la clef parcouru
		//ft_strdel(&tmp);
		if (!(tmp = get_alias_elem_by_id(g_alias, id)))		// On renvoi la value a la pos x qui deviendra notre nouvelle clef
			break ;
		if ((already_searched(key_list, tmp)))		// On envoi l'actuel valeure a la fonction qui check notre liste de clef déjà parcouru = loop infinie
		{
			//ft_strdel(&tmp);
			//ft_lstdel(&key_list, &free_key);
			return ;
		}
	}
	if ((array_key_exists(g_alias, (*av)->cmd)))
	{
		do_all_stuff(av, tmp);
	//	ft_lstdel(&key_list, &free_key);
	}
	//ft_strdel(&tmp);
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

