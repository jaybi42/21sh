/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:01:52 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/17 18:17:43 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#define TRUE 1
#define FALSE 0
/*
typedef struct		s_bin_string
{
	int	len;
	char	*string;
}			t_bin_string;

typedef struct		s_output
{
	int len;
	char *string;
	int ret_code;

}			t_output;

typedef struct		s_exec
{
	//t_bin_string	out;
	//t_bin_string	err;
	int		fd_out;
	int		fd_err;
	int		ret_code;
}			t_exec;

int	t_len2(char **argv)
{
	int i;

	i = 0;
	while (argv[i] != NULL)
	{
		i++;
	}
	return (i);
}
static void		ft_affichage(int i, const char *s)
{
	printf("%d : |%s|\n", i, s);
}

void			ft_puttab(char **tabs, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		ft_affichage(i, tabs[i]);
		i++;
	}
	ft_affichage(i, "NULL");
}

#define NEW_AV(TYPE, CMD, ARGV) ((t_av) {TYPE,CMD,(char **)ARGV, t_len2((char **)ARGV), NULL,NULL})
#define NEW_BIN_STRING(LEN, STRING) ((t_bin_string){LEN, STRING})
#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);
#define NEW_EXEC() ((t_exec){-1, -1, -1})
int	str_concat(char **s1,int *l1, char *s2, int l2)
{
	char *ns;
	int i;
	int i2;

	i = 0;
	ns = malloc(sizeof(char) * ((*l1) + l2 + 1));
	if (ns == NULL)
		return (FALSE);
	while (i < (*l1))
	{
		ns[i] = (*s1)[i];
		i++;
	}
	i2 = 0;
	while (i2 < l2)
	{
		ns[i + i2] = s2[i2];
		i2++;
	}
	ns[i + i2] = '\0';
	free((*s1));
	(*s1) = ns;
	(*l1) += l2;
	return (TRUE);
}
char	**fusion_arrays(char **arr1, int *len, char **arr2, int insert_index)
{
	char **narr;
	int nar_len;
	int i1;
	int i2;
	int ab;
	if (arr1 == NULL || arr2 == NULL)
		return (NULL);
	nar_len = (*len) + t_len2(arr2);
	narr = malloc(sizeof(char *) * (nar_len + 1));
	if (narr == NULL)
		return (NULL);
	i1 = 0;
	while (i1 < insert_index && i1 < (*len))
	{
		narr[i1] = arr1[i1];
		i1++;
	}
	i2 = 0;
	while(i2 < t_len2(arr2))
	{
		narr[i1 + i2] = arr2[i2];
		i2++;
	}
//	printf("%d/%d vs %d, %d\n", i1,(*len), i2, insert_index);
	if (i1 == insert_index)
		ab = 1;
	else
		ab = 0;
	while (i1 < (*len))
	{
		narr[i1 + i2] = arr1[i1 + ab];
		i1++;
	}
	narr[i1 + i2] = NULL;
//	printf("ainsi///\n");
//	ft_puttab(narr,nar_len);
	(*len) = nar_len - 1;
	return (narr);
}
int	do_exec_get(char *path, char **argv, t_exec *ex)
{
	int link_out[2];
	int fd_output[2];
	pid_t pid;
	int wait_status;

	if (pipe(link_out)==-1)
		die("pipe");
	if ((pid = fork()) == -1)
    		die("fork");
	if(pid == 0)
	{
		dup2 (link_out[1], STDOUT_FILENO);
		close(link_out[0]);
		close(link_out[1]);
		execve(path, argv, NULL);
		die("execve");
	}
	else
	{
		close(link_out[1]);
		//printf("fd_out = %d | fd_err = %d\n", link_out[0], link_err[0]);
		ex->fd_out = link_out[0];
		//oklm
		ex->ret_code = waitpid(-1, &wait_status, WUNTRACED);
		if (WIFEXITED(wait_status))
			ex->ret_code = WEXITSTATUS(wait_status);
	}
	return (1);
}
t_exec exec_no_pipe(int fd_stdout, char *path, char **argv, t_redirect **red, int ret)
{
	t_exec ex;
	int **links;
	int i;
	pid_t pid;
	int fd_out;
	fd_out = -1;
	int fd_err;
	fd_err = -1;
	int wait_status;

	if ((pid = fork()) == -1)
    		die("fork");
	if(pid == 0)
	{
		i = -1;
		if (red != NULL)
			while (red[++i] != NULL)
			{
				if (red[i]->fd_in == 1)
					fd_out = dup(1);
				if (red[i]->fd_in == 2)
					fd_err = dup(2);
				dup2(red[i]->fd_out, red[i]->fd_in);
			}
		execve(path, argv, NULL);
		die("execve");
	}
	else
	{
		//printf("fd_out = %d | fd_err = %d\n", link_out[0], link_err[0]);
		//ex->fd_out = link_out[0];
		//ex->fd_err = link_err[0];
		//oklm
		ex.ret_code = waitpid(-1, &wait_status, WUNTRACED);
		if (WIFEXITED(wait_status))
			ex.ret_code = WEXITSTATUS(wait_status);
	}
	i = -1;
	if (red != NULL)
		while (red[++i] != NULL)
			close(red[i]->fd_out);
	if (fd_out != -1)
		dup2(fd_out, 1);
	if (fd_err != -1)
		dup2(fd_err, 2);
	return (ex);
}

t_bin_string	exec_av(t_av **av, int ret)
{
	t_bin_string s_ret;
	t_bin_string s_tmp;
	int i_argv;
	int i_argcmd;
	int a;
	t_exec ex;

	a = 0;
	ex = NEW_EXEC();
	s_ret.string = malloc(sizeof(char));
	s_ret.string[0] = '\0';
	s_ret.len = 0;
	while(av[a] != NULL)
	{
		//we are ready for launch the exec...
		if (ret == TRUE)
		{
				ex = NEW_EXEC();
				do_exec_get(av[a]->cmd, av[a]->argv, &ex);
				fd_get_binary(ex.fd_out, &s_tmp.string, &s_tmp.len);
				printf("output tmp: |%.*s|\n", s_tmp.len, s_tmp.string);
				str_concat(&s_ret.string, &s_ret.len, s_tmp.string, s_tmp.len);
		}
		else
		{
			if (av[a]->type != TYPE_PIPE)
			{
				exec_no_pipe(0, av[a]->cmd, av[a]->argv, av[a]->redirect, FALSE);
			}
		}
		a++;
	}
	if (ret == TRUE)
		return (s_ret);
	else
		return (NEW_BIN_STRING(-1, NULL));
}

int	update_bquote(t_av **av, char ***argv, int *len, int index)
{
	char **array_to_add;

	t_bin_string out = exec_av(av, TRUE);
	printf("output: %.*s\n", out.len, out.string);
	array_to_add = fstrsplit(out.string, out.len, char_is_whitespace);
	//printf("splitted array = ..\n");
	//ft_puttab(array_to_add, t_len(array_to_add));
	(*argv) = fusion_arrays((*argv), len, array_to_add, index);
	if ((*argv) != NULL)
		return (TRUE);
	else
		return (FALSE);
}

void	check_av(t_av ***av)
{
	int a;
	int i_argv;
	int i_argcmd;

	a = 0;
	printf("check_av:\n");
	while ((*av)[a] != NULL)
	{
		i_argv = 0;
		i_argcmd = 0;
		while (i_argv < (*av)[a]->argc)
		{
			if ((*av)[a]->argv[i_argv] == NULL)
			{
			printf("beg\n");
			ft_puttab((*av)[a]->argv, (*av)[a]->argc);
			update_bquote((*av)[a]->argcmd[i_argcmd++], &(*av)[a]->argv,&(*av)[a]->argc, i_argv);
			printf("after\n");
			ft_puttab((*av)[a]->argv, (*av)[a]->argc);
			}
			i_argv++;
		}
		a++;
	}
}
*/

/*
** Try the builtin
** if nothing match try to fork, execve
** if -1, exit has been typed
*/

int				shell(t_av **av, t_list **g_env, t_list **l_env, t_ftl_root *hist)
{
	int i;
	int ret;

//	check_av(&av);
	i = -1;
	while (av[++i] != NULL)
	{
		if (av[i]->argv[0] == NULL)
			continue;
		if ((ret = builtin(g_env, l_env, hist, av[i])) == 1)
			ret = check_bin(*g_env, *l_env, (*av)[i]);
		else if (ret == -1)
			return (1);
	}
	return (0);
}
