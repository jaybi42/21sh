/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/11 22:58:52 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/11 23:06:26 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

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
