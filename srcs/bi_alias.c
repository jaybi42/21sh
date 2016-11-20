/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_alias.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/19 16:21:54 by ibouchla          #+#    #+#             */
/*   Updated: 2016/11/19 19:32:49 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct		s_alias
{
	char			*key;
	char			*value;
	struct s_alias	*next;
}					t_alias;

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

int	print_alias_key(t_alias *alias, char *key)
{
	while (alias != NULL)
	{
		if ((ft_strcmp(alias->key, key)) == 0)
		{
			ft_putstr(alias->key);
			ft_putchar('=');
			ft_putendl(alias->value);		
			return (0);
		}
		alias = alias->next;
	}
	return (1);	
}

int	array_key_exists(t_alias *alias, char *key)
{
	while (alias != NULL)
	{
		if ((ft_strcmp(alias->key, key)) == 0)
			return (1);
		alias = alias->next;
	}
	return (0);
}

int	print_alias_list(t_alias *alias, char *key)
{
	if (key != NULL)
		return ((array_key_exists(alias, key)) ? print_alias_key(alias, key) : 1);
	while (alias != NULL)
	{
		ft_putstr(alias->key);
		ft_putchar('=');
		ft_putendl(alias->value);
		alias = alias->next;
	}
	return (0);
}

char	*strdup_aplha(char *cmd)
{
	int		i;
	char	*ret;

	i = 1;
	if (cmd == '\0')
		return(ft_strdup("\0"));
	while ((ft_isalpha(cmd[i])) && cmd[i] != '\0')
		++i;
	if (!(ret = ft_strnew(i)))
		return ((char *)NULL);
	i = 1;
	while ((ft_isalpha(cmd[i])) && cmd[i] != '\0')
	{
		ret[i - 1] = cmd[i];
		++i;
	}
	ret[i - 1] = '\0';
	return (ret);
}

char	**get_data(char *cmd)
{
	char	**pair;

	if (!(pair = (char **)ft_memalloc(sizeof(char *) * 2)))
		return ((char **)NULL);
	while ((ft_isspace(cmd[i])))
		++i;
	pair[0] = strdup_aplha((cmd + i));
	pair[1] = ((cmd[i] == '=') ? NULL : strdup_aplha((cmd + i)));
	return (pair);
}

int	bi_alias(char *cmd, int argc)
{
	int		x;
	char	**pair;

	x = 0;
	if (argc == 1)
		return (print_alias_list((char *)NULL));
	while (arg[++x] != NULL)
	{
		if ((pair = get_data(cmd)))
		{
			if (pair[1] == NULL)
				if ((print_alias_list(pair[0])) != 0)
					ft_putendl_fd("21sh: Alias not found", 2);
			del_pairs();
		}
	}
	return (0);
}

int	main(void)
{
	bi_alias("alias l=i", 2)
}
