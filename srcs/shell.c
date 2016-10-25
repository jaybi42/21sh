/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:01:52 by jguthert          #+#    #+#             */
/*   Updated: 2016/10/23 18:41:51 by agadhgad         ###   ########.fr       */
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


#define TYPE_REDIRECT_NORMAL 0
#define TYPE_REDIRECT_TRUNC 1

typedef struct s_f
{
	    int fd[2];
		    int oldout;
			    int tempout;
				    int fd_to_redir;
					    int fd_to_write;
}               t_f;

static t_builtin const  g_builtin_list[8] = {
	{"cd", bi_cd},
	{"setenv", bi_setenv},
	{"unsetenv", bi_unsetenv},
	{"env", bi_env},
	{"getenv", bi_getenv},
	{"echo", bi_echo},
	{"exit", bi_exit},
	{"history", bi_history},
};

char *get_path(t_list *g_env, t_list *l_env)
{
	while (g_env != NULL)
	{
		if (ft_strcmp(((t_env *)g_env->content)->name, "PATH") == 0)
			return (((t_env *)g_env->content)->value);
		g_env = g_env->next;
	}
	while (l_env != NULL)
	{
		if (ft_strcmp(((t_env *)l_env->content)->name, "PATH") == 0)
			return (((t_env *)l_env->content)->value);
		l_env = l_env->next;
	}
	return (NULL);
}

char **convert_env(t_list *g_env, t_list *l_env)
{
	t_list	*env;
	char	**tab;
	int		i;

	env = g_env;
	if (env == NULL)
		env = l_env;
	i = ft_listlen(env);
	tab = (char **)malloc(sizeof(char *) * (i + 1));
	if (tab == NULL)
		return (NULL);
	tab[i--] = NULL;
	while (env != NULL)
	{
		tab[i--] = ft_strdup(((t_env *)env->content)->str);
		env = env->next;
	}
	return (tab);
}

char **get_allpath(char *cmd, char *path)
{
	char	**temp;
	char	**allpath;
	int		i;

	i = -1;
	temp = ft_strsplit(path, ':');
	allpath = (char **)malloc(sizeof(char *) * (ft_tablen(temp) + 1));
	while (temp[++i] != NULL)
		allpath[i] = ft_strjoin(temp[i], "/");
	allpath[i] = NULL;
	ft_tabdel(temp);
	temp = (char **)malloc(sizeof(char *) * (ft_tablen(allpath) + 1));
	i = -1;
	while (allpath[++i] != NULL)
		temp[i] = ft_strjoin(allpath[i], cmd);
	temp[i] = NULL;
	ft_tabdel(allpath);
	return (temp);
}

int	 exec_path(char **arg, char **path, char **env)
{
	int i;

	i = 0;
	while (path[i] != NULL)
	{
		if (access(path[i], X_OK) != -1)
			return (i);
		i++;
	}
	return (-1);
}


t_exec		make_exec_builtin(t_av *av)
{
	t_exec ex;
	int i;

	i = 0;
	ex.type = -1;
	while (i < 8)
	{
		if (ft_strcmp(g_builtin_list[i].key, av->cmd) == 0)
		{
			ex.fnct = g_builtin_list[i].value;
			ex.argv = av->argv;
			ex.path = NULL;
			ex.type = BUILTIN;
			break;
		}
		i++;
	}
	return (ex);
}

t_exec make_exec_bin(t_av *av, t_list *g_env, t_list *l_env)
{
	t_exec ex;
	char	**env;
	char	**path;
	char	*str;
	int		ret;

	ex.type = -1;
	ex.fnct = NULL;
	env = convert_env(g_env, l_env);
	str = get_path(g_env, l_env);
	path = get_allpath(av->cmd, str);
	if (access(av->cmd, X_OK) != -1)
	{
		ex.path = av->cmd;
		ex.argv = av->argv;
	}
	else
	{
		ret = exec_path(av->argv, path, env);
		if (ret != -1)
		{
			ex.type = BIN;
			ex.path = path[ret];
			ex.argv = av->argv;
		}
	}
	return (ex);
}

/*
 ** Try the builtin
 ** if nothing match try to fork, execve
 ** if -1, exit has been typed
 */
t_exec make_exec(t_av *av, t_list **g_env, t_list **l_env)
{
	t_exec ex;

	ex = make_exec_builtin(av);
	if (ex.type == -1)
	{ex = make_exec_bin(av, *g_env,*l_env);
	}
		ex.r = av->redirect;
	return (ex);
}

/*
t_output	**exec_tav(t_av **av, int ret)
{

}*/

int		x_strjoins(char **s1, size_t *len1, char *s2, size_t len2)
{
	int i;
	int a;
	char *s;

	a = 0;
	int len;
	if (s1 == NULL || s2 == NULL)
		return (0);
	len = (int)((*len1) + len2 + 1);
	s = malloc(sizeof(char) * len);
	if (!s)
		return (0);
	i = -1;
	while (++i < (int)(*len1))
		s[a++] = (*s1)[i];
	i = -1;
	while (++i < (int)len2)
		s[a++] = s2[i];
	s[a] = '\0';
	(*s1) = s;
	(*len1) = a;
	return (1);
}

int		exec_builtin(int (*fnct)(), char **argv, t_redirect **r,
		t_av av)
{
	t_f **f;
	int a = 0;
	int i = 0;
	t_output o;
	while (r[i])i++;
	if (!(f = malloc(sizeof(t_f *) * (i + 1)))) { printf("error"); exit(4);}
	i = -1;
	a = 0;
	while (r[++i])
		if (r[i]->type == 0)
		{
			f[a] = malloc(sizeof(t_f) * (2));
			f[a]->fd_to_redir = r[i]->fd_in;
			f[a]->fd_to_write = r[i]->fd_out;
			pipe(f[a++]->fd);
		}
	f[a] = NULL;
	i = 0;
	while (f[i])
	{
		if (r[i]->type == 0)
		{
		f[i]->oldout = dup(f[i]->fd_to_redir);
		close(f[i]->fd_to_redir);
		f[i]->tempout = dup(f[i]->fd[1]);
		}
		i++;
	}

	fnct(av, &g_env, &l_env);

	i = 0;
	while (f[i])
	{
		close(f[i]->fd[1]);
		close(f[i]->fd_to_redir);
		fd_get_binary(f[i]->fd[0], &o.string, &o.len);
		close(f[i]->fd[0]);
		write(f[i]->fd_to_write, o.string, o.len);
		f[i]->tempout = dup(f[i]->oldout);
		close(f[i]->oldout);
		i++;
	}
	return (0);
}

int		exec_bin(char *path, char **argv, t_redirect **r, char *in, int inlen)
{
	int ret;
	int wait_status;
	pid_t pid;
	int fd[2];
	t_f **f;
	int i;
	t_output o;
	int a;

	if (inlen > 0)
		pipe(fd);
	i = 0;
	ret = -1;
	while (r[i])i++;
	if (!(f = malloc(sizeof(t_f *) * (i + 1)))) { printf("error"); exit(4);}
	i = -1;
	a = 0;
	while (r[++i]){
		if (r[i]->type != 0)
			continue;
		f[a] = malloc(sizeof(t_f) * (2));
		f[a]->fd_to_redir = r[i]->fd_in;
		f[a]->fd_to_write = r[i]->fd_out;
		pipe(f[a++]->fd);
		//dprintf(2, "pipe %d<-%d\n", f[i]->fd[0], f[i]->fd[1]);
	}
	f[a] = NULL;
	if ((pid = fork()) == -1)
	{
		printf("error\n");
		exit(0);
	}
	if (pid == 0)
	{
		i = 0;
		while (f[i])
		{
			dup2(f[i]->fd[1], f[i]->fd_to_redir);
			i++;
		}
		i = 0;
		while (f[i])
		{
			close(f[i]->fd[1]);
			close(f[i]->fd[0]);
			i++;
		}
		if (inlen > 0)
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
		}
		execve(path, argv, NULL);
	}
	else
	{
		/*
		i = 0;
		while (f[i])
		{
	//		close(f[i]->fd[1]);
			i++;
		}*/
		if (inlen > 0)
		{
			close(fd[0]);
			write(fd[1], in, inlen); //writing in stdin
			close(fd[1]);
		}
		ret = waitpid(-1, &wait_status, WUNTRACED);
		if (WIFEXITED(wait_status))
			ret = WEXITSTATUS(wait_status);
		i = 0;
		while (f[i])
		{
			close(f[i]->fd[1]);
			//dprintf(2, "open %d\n", f[i]->oldout);
			int last = i;
			/*
			int x = 0;
			while (f[x])
			{
				if (f[i]->fd_to_redir == f[x]->fd_to_redir)
					last = x;
				x++;
			}*/
			fd_get_binary(f[last]->fd[0], &o.string, &o.len);
			close(f[i]->fd[0]);
			write(f[i]->fd_to_write, o.string, o.len);
		//	f[i]->tempout = dup(f[i]->oldout); //reopen the closen one
		//	close(f[i]->oldout);
			i++;
		}
	}
	return (ret);
}

typedef struct s_h
{
	int x;
	int fd[2];
}							t_h;

int get_out(t_redirect ***r, int fd_out)
{
	int i = 0;
	while((*r)[i])i++;
	(*r)[i] = malloc(sizeof(t_redirect));
	(*r)[i]->type = 0;
	(*r)[i]->fd_in = 1;
	(*r)[i]->fd_out = fd_out;
	//(*r)[i]->fd_out = open("okalm.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
	(*r)[i + 1] = NULL;
	return (1);
}

int	set_in(t_redirect **r, char **s, size_t len)
{
	int i;

	i = 0;
	while (r[i])
	{
		if (r[i]->type == 1)
		{
			int ret = x_strjoins(s, &len, r[i]->s_in, r[i]->len_in);
		}
		i++;
	}
	return (len);
}

t_output do_exec(t_exec ex, int ret,
		t_av av, char *in, int inlen)
{
	t_output o;
	int fd[2];

	if (ret != 0)
	{
		pipe(fd);
		get_out(&ex.r, fd[1]);
	}
	inlen = set_in(ex.r, &in, (size_t)inlen);
	//dprintf(2, "putting in stdin: %d|%.*s|\n",inlen, inlen, in);
	o.ret_code = 0;
	if (ex.type == BUILTIN)
		o.ret_code = exec_builtin(ex.fnct, ex.argv, ex.r,av);
	else
		o.ret_code = exec_bin(ex.path, ex.argv, ex.r, in, inlen);
	if (ret != 0)
	{
		int i = 0;
		while (ex.r[i]) {
			if (ex.r[i + 1] == NULL)
				break;
			if (ex.r[i]->fd_in > 2)
				close(ex.r[i]->fd_in);
			if (ex.r[i]->fd_out > 2)
				close(ex.r[i]->fd_out);
			i++;
		}
		close(fd[1]);
		fd_get_binary(fd[0], &o.string, &o.len);
		close(fd[0]);
		//dprintf(2, "read --> |%.*s|\n", o.len, o.string);
	}
	return (o);
}

void clear_output(t_output *o)
{
	o->len = 0;
	o->string = xmalloc(2);
	if (o->string == NULL)
		{dprintf(2, "malloc error.. leaving...\n");exit(1);}
	o->string[0] = '\0';
}

t_output		shell(t_av **av, int ret)
{
	int fd_in[2];
	int i_cmd;
	int a;
	t_exec ex;
	t_output all;
	t_output output;
	int find = -1;

	a = -1;
	clear_output(&all);
	clear_output(&output);
	output.string[0] = '\0';
	output.ret_code = 0;
	while (av[++a] != NULL)
	{
		if (av[a]->argv[0] == NULL)
			continue;
		ex = make_exec(av[a], &g_env, &l_env);
		//check AND and OR
		if (av[a]->type == TYPE_OR || av[a]->type == TYPE_AND)
		{
			if (find == 0)
			{
				if ((output.ret_code == 0 && av[a]->type == TYPE_OR) ||
					(output.ret_code != 0 && av[a]->type == TYPE_AND))
				{
					find = 1;
					continue;
				}
			}
			else
				continue;
		}
		if (av[a + 1] != NULL && (av[a + 1]->type == TYPE_OR || av[a + 1]->type == TYPE_AND)
			&& (find == -1))
					find = 0;
		else
					find = -1;
		if (ex.type == -1)
				{	ft_printf("unknown command bro\n");output.ret_code = 1; continue;}
		if (av[a]->type != TYPE_PIPE)
			clear_output(&output);
		if (av[a]->type == TYPE_PIPE && output.ret_code != 0)
			continue;
		output = do_exec(ex, (av[a + 1] != NULL && av[a + 1]->type == TYPE_PIPE) ? 1 : ret, *av[a],
			output.string, output.len);

		if (ret)
			x_strjoins(&all.string,(size_t *)&all.len,output.string,output.len);
		all.ret_code = output.ret_code; //get the last retcode
	}
	return (all);
}

/*
** give him an expr (example: echo ok | cat -e)
** give you the return output and the last ret_code
*/
t_output shell_exec(char *expr)
{
	t_av **av;
	t_output o;
	av = parse_commands(expr);
	o = shell(av, 1);
	return (o);
}
