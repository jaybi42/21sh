/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:36:15 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/11 15:50:22 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int		set_redir_init2(t_set_redir *t, t_av **pcmd)
{
	if (!in_cmd_is_intouchable((t->cmd), (t->id_argv), (t->i) + (t->len))
		&& (t->cmd)->argv[(t->id_argv)][(t->i) + (t->len)] == '&')
	{
		(t->findfile) = FALSE;
		delete_c(pcmd, (t->id_argv), (t->i) + (t->len), &(t->i));
	}
	if ((t->cmd)->argv[(t->id_argv)][(t->i) + (t->len)] == '\0')
	{
		(t->i) = 0;
		(t->len) = 0;
		if (ft_strlen((t->cmd)->argv[(t->id_argv)]) == 0)
		{
			delete_s(pcmd, (t->id_argv));
			(t->id_argv)--;
		}
		(t->id_argv)++;
	}
	if ((t->cmd)->argv[(t->id_argv)] == NULL)
	{
		print_err("parse error with the redirection", "Parsing");
		return (FALSE);
	}
	return (TRUE);
}

void	set_redir_inside(t_set_redir *t, t_av **pcmd)
{
	(t->x) = 0;
	while ((t->cmd)->argv[(t->id_argv)][(t->i) + (t->len)])
	{
		(t->s)[(t->x)] = (t->cmd)->argv[(t->id_argv)][(t->i) + (t->len)];
		if ((is_redir((t->cmd)->argv[(t->id_argv)] + (t->i) + (t->len))
		&& !in_cmd_is_intouchable((t->cmd), (t->id_argv), (t->i))))
			break ;
		delete_c(pcmd, (t->id_argv), (t->i) + (t->len), &(t->i));
		(t->i)++;
		(t->x)++;
	}
	(t->s)[(t->x)] = '\0';
	if (ft_strlen((t->cmd)->argv[(t->id_argv)]) == 0)
	{
		delete_s(pcmd, (t->id_argv));
		(t->id_argv)--;
	}
	if ((t->findfile) == FALSE && !(ft_strlen((t->s)) > 0
		&& !ft_isdigit((t->s)[0])))
		(t->findfile) = TRUE;
}

void	set_redir_inside2(t_set_redir *t, char *r)
{
	if (ft_strncmp(r, ">", 1) == 0)
	{
		if ((t->findfile) == FALSE)
			(t->redir)->fd = ft_atoi((t->s));
		else
		{
			(t->redir)->path = (t->s);
			(t->redir)->open_flag = ((ft_strcmp(r, ">>") == 0)
			? O_CREAT | O_RDWR | O_APPEND : O_CREAT | O_RDWR | O_TRUNC);
		}
	}
	else
	{
		(t->redir)->type = 1;
		if (ft_strcmp(r, "<<") == 0)
			(t->redir)->fd = get_heredoc((t->s));
		else
		{
			(t->redir)->path = (t->s);
			(t->redir)->open_flag = O_RDONLY;
		}
	}
	(g_debug) ? ft_dprintf(2, "{yellow}[?]{eoc} add a redirection '%s'\
	from open(\"%s\")\n", r, (t->s)) : 0;
}

int		set_redir(t_av **pcmd, int id_argv, int i, char *r)
{
	t_set_redir t;

	(t.i) = i;
	(t.id_argv) = id_argv;
	if (!set_redir_init(&t, pcmd, r))
		return (FALSE);
	if (!set_redir_init2(&t, pcmd))
		return (FALSE);
	if (!((t.s) = xmalloc(sizeof(char *) *
		(ft_strlen((t.cmd)->argv[(t.id_argv)]) + 1))))
		return (FALSE);
	set_redir_inside(&t, pcmd);
	set_redir_inside2(&t, r);
	(t.i) = 0;
	while ((t.cmd)->redirect[(t.i)])
		(t.i)++;
	(t.cmd)->redirect[(t.i)] = (t.redir);
	(t.cmd)->redirect[(t.i) + 1] = NULL;
	return (TRUE);
}

int		get_redirect(t_av **cmd)
{
	int		id_argv;
	char	*s;
	int		i;
	char	*tmp;

	id_argv = 0;
	while ((*cmd)->argv[id_argv] != NULL)
	{
		s = (*cmd)->argv[id_argv];
		i = -1;
		while (s[++i])
		{
			if (in_cmd_is_intouchable((*cmd), id_argv, i))
				continue ;
			if ((tmp = is_redir(s + i)))
			{
				if (!set_redir(cmd, id_argv, i, tmp))
					return (FALSE);
				id_argv = -1;
				break ;
			}
		}
		id_argv++;
	}
	return (TRUE);
}
