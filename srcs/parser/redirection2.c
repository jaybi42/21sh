/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 23:10:48 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/11 19:16:55 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int		set_redir_init(t_set_redir *t, t_av **pcmd, char *r)
{
	(t->cmd) = (*pcmd);
	(t->fd) = -1;
	(t->redir) = xmalloc(sizeof(t_redirect));
	(t->redir)->fd_in = 1;
	(t->redir)->fd_out = -1;
	(t->redir)->fd = -1;
	(t->redir)->type = 0;
	if ((t->i) > 0 && !in_cmd_is_intouchable((t->cmd), (t->id_argv), (t->i) - 1)
			&& ft_isdigit((t->cmd)->argv[(t->id_argv)][(t->i) - 1]))
	{
		(t->redir)->fd_in = (t->cmd)->argv[(t->id_argv)][(t->i) - 1] - 0x30;
		delete_c(pcmd, (t->id_argv), (t->i) - 1, &(t->i));
	}
	(t->len) = ft_strlen(r);
	(t->b) = -1;
	while ((++(t->b)) < (int)ft_strlen(r))
		delete_c(pcmd, (t->id_argv), (t->i) + (t->b), &(t->i));
	if (!(t->cmd)->argv[(t->id_argv)])
		return (FALSE);
	(t->findfile) = TRUE;
	return (TRUE);
}

char	*is_redir(char *s)
{
	int				i;
	static char		*redirs[] = {">>", ">", "<<", "<", NULL};

	i = 0;
	while (redirs[i])
	{
		if (ft_strncmp(s, redirs[i], ft_strlen(redirs[i])) == 0)
			return (redirs[i]);
		i++;
	}
	return (NULL);
}

int		get_heredoc(char *s)
{
	char	*tmp;
	int		fd[2];

	pipe(fd);
	tmp = NULL;
	while (42)
	{
		tmp = get_input("heredoc> ");
		if (ft_strcmp(s, tmp) == 0)
			break ;
		else
		{
			write(fd[WRITER], tmp, ft_strlen(tmp));
			write(fd[WRITER], "\n", 1);
		}
	}
	close(fd[WRITER]);
	return (fd[READER]);
}
