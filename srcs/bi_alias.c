/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_alias.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/19 16:21:54 by ibouchla          #+#    #+#             */
/*   Updated: 2016/11/20 17:48:25 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*

 LA SUBSTITUTION DE L'ALIAS N'EST EFFECTUER QU'AU MOMENT DE L'EXECUTION, C'EST D'AILLEUR LA PREMIERE ETAPE

 **	alias | print la liste des alias
 **	alias ls | print la valeure de ls ou si il n'existe pas print msg d'erreur code retour >0
 ** alias name=value | comportement classique ou meme 'name=' deviendra alias name=""

	Alias "1" VALUE "2"
			Alias "2" VALUE "3"
					Alias "3" value "X"
							Alias "X" VALUE (1, 2 ou 3) alors boucle infinie sinon on continue

 Si il y a une boucle infinie type a->b; b->c; c->a; alors aucun remplacement sur l'alias n'est effectue.
 Sinon tant que il y a encore un alias 'name' qui vaut la value de l'alias donner en arg on continue, puis on remplace l'alias par la value

*/

int		print_alias_key(t_alias *alias, char *key)
{
	while (alias != NULL)
	{
		if ((ft_strcmp(alias->key, key)) == 0)
		{
			ft_putstr(alias->key);
			ft_putstr("=\'");
			ft_putstr(alias->value);
			ft_putendl("\'");		
			return (0);
		}
		alias = alias->next;
	}
	return (1);	
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

int		print_alias_list(t_alias *alias, char *key)
{
	if (key != NULL)
		return ((array_key_exists(alias, key)) ? print_alias_key(alias, key) : 1);
	while (alias != NULL)
	{
		ft_putstr(alias->key);
		ft_putstr("=\'");
		ft_putstr(alias->value);
		ft_putendl("\'");
		alias = alias->next;
	}
	return (0);
}

char	*strdup_aplha(char *cmd)
{
	int		i;
	char	*ret;

	i = 0;
	if (cmd == '\0')
		return (ft_strdup("\0"));
	while ((ft_isalpha(cmd[i])) && cmd[i] != '\0')
		++i;
	if (!(ret = ft_strnew(i)))
		return ((char *)NULL);
	i = 0;
	while ((ft_isalpha(cmd[i])) && cmd[i] != '\0')
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
	char	**pair;

	i = 0;
	if (!(pair = (char **)ft_memalloc(sizeof(char *) * 2)))
		return ((char **)NULL);
	while ((ft_isspace(cmd[i])) && cmd[i])
		++i;
	pair[0] = strdup_aplha((cmd + i));
	if (cmd[i] == '=')
		pair[1] = NULL;
	else
	{
		while (cmd[i] != '=' && cmd[i] != '\0')
			++i;
		i = ((cmd[i] == '=') ? i + 1 : i);
		pair[1] = strdup_aplha((cmd + i));
	}
	return (pair);
}

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

int			bi_alias(t_av av, t_list **g_env, t_list **l_env)
{
	char *cmd = av.cmd;
	int argc = av.argc;
	(void)g_env;
	(void)l_env;
	int		x;
	char	**pair;
	t_alias	*alias = NULL;

	ft_dprintf(2, "Ilyes batar\n");
	x = 0;
	if (argc == 1)
		return (print_alias_list(alias, (char *)NULL));
	while (av.argv[++x] != NULL)
	{
		if ((pair = get_data(cmd)))
		{
			if (pair[1] == NULL)
			{
				if ((print_alias_list(alias, pair[0])) != 0)
					ft_putendl_fd("21sh: alias not found.", 2);
			}
			else
				create_or_update_key(&alias, pair);
			del_pair(&pair);
		}
	}
	return (0);
}
