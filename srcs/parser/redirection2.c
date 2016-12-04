#include "21sh.h"

char *is_redir(char *s)
{
	int i;
	char *redirs[] = {">>", ">", "<<", "<", NULL};

	i = 0;
	while (redirs[i])
	{
		if (ft_strncmp(s, redirs[i], ft_strlen(redirs[i])) == 0)
			return (redirs[i]);
		i++;
	}
	return (NULL);
}

int get_heredoc(char *s)
{
	char *tmp;
	int fd[2];

	pipe(fd);
	tmp = NULL;
	while (1)
	{
		tmp = get_input("heredoc> ");
		if (ft_strcmp(s, tmp) == 0)
			break;
		else
		{
			write(fd[WRITER], tmp, ft_strlen(tmp));
			write(fd[WRITER], "\n", 1);
		}
		ft_strdel(&tmp);
	}
	ft_strdel(&tmp);
	close(fd[WRITER]);
	return (fd[READER]);
}
