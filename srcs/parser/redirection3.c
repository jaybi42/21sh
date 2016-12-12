/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/11 22:58:52 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/12 17:24:07 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*defragement(char *s)
{
	int		i;
	int		a;
	char	*s2;

	if (!(s2 = xmalloc(sizeof(char) * (ft_strlen(s) + 1))))
		return (NULL);
	a = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] == ' ')
		{
			while (s[i] == ' ')
				i++;
			if (s[i] != '\0')
				s2[a++] = ' ';
			i--;
		}
		else
			s2[a++] = s[i];
	}
	s2[a] = '\0';
	return (s2);
}

void	set_redir_inside_bis(t_set_redir *t, t_av **pcmd)
{
	delete_s(pcmd, (t->id_argv));
	(t->id_argv)--;
}

void	set_redir_inside2_bis(t_set_redir *t, char *r)
{
	if ((t->findfile) == FALSE)
	{
		(t->redir)->fd = (t->fd);
		(t->redir)->path = NULL;
	}
	else
	{
		(t->redir)->path = (t->s);
		(t->redir)->open_flag = ((ft_strcmp(r, ">>") == 0)
				? O_CREAT | O_RDWR | O_APPEND : O_CREAT | O_RDWR | O_TRUNC);
	}
}
