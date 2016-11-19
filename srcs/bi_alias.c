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
	char			*name;
	char			*value;
	struct s_alias	*next;
}					t_alias;

/*
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

int	bi_alias(void)
{
	(void)g_env;
	(void)av;
	(void)l_env;
	ft_putstr("ON RENNNNNNNNNNNTRE");
	return (0);
}

l = ls -laH
zz = l;

int	main(void)
{
	bi_alias("alias l=i")
}
