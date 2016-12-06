/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:36:15 by agadhgad          #+#    #+#             */
/*   Updated: 2016/12/06 19:45:26 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int set_redir(t_av **pcmd, int id_argv, int i, char *r)
{
	char	*s;
	t_av	*cmd;
	int		findfile;
	t_redirect *redir;
	int len;

	cmd = (*pcmd);
	redir = xmalloc(sizeof(t_redirect));
	redir->fd_in = 1;
	redir->fd_out = -1;
	redir->fd = -1;
	redir->type = 0;
	if (i > 0 && !in_cmd_is_intouchable(cmd, id_argv, i - 1)
			&& ft_isdigit(cmd->argv[id_argv][i - 1]))
	{
		redir->fd_in = cmd->argv[id_argv][i-1] - 0x30;
		delete_c(pcmd, id_argv, i - 1, &i);
	}
	len = ft_strlen(r);
	int b;
	b = -1;
	while ((++b) < (int)ft_strlen(r))
		delete_c(pcmd, id_argv, i + b, &i);
	if (!cmd->argv[id_argv])
		return (FALSE);
	findfile = TRUE;
	if (!in_cmd_is_intouchable(cmd, id_argv, i + len) && cmd->argv[id_argv][i + len] == '&')
	{
		findfile = FALSE;
		delete_c(pcmd, id_argv, i + len, &i);
	}
	if (cmd->argv[id_argv][i + len] == '\0')
	{
		i = 0;
		len = 0;
		if (ft_strlen(cmd->argv[id_argv]) == 0)
		{
			delete_s(pcmd, id_argv);
			id_argv--;
		}
		id_argv++;
	}
	if (cmd->argv[id_argv] == NULL)
	{
		print_err("parse error with the redirection", "Parsing");
		return (FALSE);
	}
	int x;
	s = xmalloc(sizeof(char *) * (ft_strlen(cmd->argv[id_argv]) + 1));
	x = 0;
	while (cmd->argv[id_argv][i + len])
	{
		s[x] = cmd->argv[id_argv][i + len];
		if ((is_redir(cmd->argv[id_argv] + i + len) &&
					!in_cmd_is_intouchable(cmd,id_argv,i)))//|| (findfile == FALSE && !is_d[x]))
						break;
		delete_c(pcmd, id_argv, i + len, &i);
		i++;
		x++;
	}
	s[x] = '\0';
	if (ft_strlen(cmd->argv[id_argv]) == 0)
	{
		delete_s(pcmd, id_argv);
		id_argv--;
	}
	if (findfile == FALSE && !(ft_strlen(s) > 0 && !ft_isdigit(s[0])))
		findfile = TRUE;
	if (ft_strncmp(r, ">", 1) == 0)
	{
		if (findfile == FALSE)
			redir->fd = ft_atoi(s);
		else
		{
			redir->path = s;
			redir->open_flag = ((ft_strcmp(r, ">>") == 0) ? O_CREAT | O_RDWR | O_APPEND : O_CREAT | O_RDWR | O_TRUNC);
		}
	}
	else
	{
		redir->type = 1;
		if (ft_strcmp(r, "<<") == 0)
			redir->fd = get_heredoc(s);
		else
		{
			redir->path = s;
			redir->open_flag = O_RDONLY;
		}
	}
	(g_debug) ? ft_dprintf(2, "{yellow}[?]{eoc} add a redirection '%s' from open(\"%s\")\n", r, s) : 0;
	i = 0;
	while (cmd->redirect[i])
		i++;
	cmd->redirect[i] = redir;
	cmd->redirect[i + 1] = NULL;
	return (TRUE);
}

int get_redirect(t_av **cmd)
{
	int id_argv;
	char *s;
	int i;
	char *tmp;

	id_argv = 0;
	while((*cmd)->argv[id_argv] != NULL)
	{
		s = (*cmd)->argv[id_argv];
		i = -1;
		while(s[++i])
		{
			if (in_cmd_is_intouchable((*cmd), id_argv, i))
				continue;
			if ((tmp = is_redir(s + i)))
			{
				if (!set_redir(cmd, id_argv, i, tmp))
					return (FALSE);
				id_argv = -1;
				break;
			}
		}
		id_argv++;
	}
	return (TRUE);
}
