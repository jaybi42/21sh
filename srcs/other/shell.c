/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/16 17:01:52 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/23 16:51:04 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include "autocomp.h"
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

#define NB_BUILT 10

static t_builtin const  g_builtin_list[NB_BUILT] = {
	{"cd", bi_cd},
	{"setenv", bi_setenv},
	{"unsetenv", bi_unsetenv},
	{"env", bi_env},
	{"getenv", bi_getenv},
	{"echo", bi_echo},
	{"exit", bi_exit},
	{"clear", bi_clear},
	{"history", bi_history},
	{"alias", bi_alias}
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
void print_err(char *err, char *what)
{
	ft_dprintf(2, "%s: %s: %s\n", NAME, err, what);
}

char **convert_env(t_list *g_env, t_list *l_env)
{
	t_list	*env;
	char	**t;
	int		i;

	env = g_env;
	if (env == NULL)
		env = l_env;
	i = ft_listlen(env);
	t = (char **)xmalloc(sizeof(char *) * (i + 1));
	if (t == NULL)
		return (NULL);
	t[i--] = NULL;
	while (env != NULL)
	{
		t[i--] = x_strdup(((t_env *)env->content)->str);
		env = env->next;
	}
	return (t);
}

char **get_allpath(char *cmd, char *path)
{
	char	**temp;
	char **temp2;
	char	**allpath;
	int		i;
	int		a;

	i = -1;
	temp = ft_strsplit(path, ':');
	allpath = (char **)xmalloc(sizeof(char *) * (ft_tablen(temp) + 1));
	while (temp[++i] != NULL)
		allpath[i] = x_strjoin(temp[i], "/");
	allpath[i] = NULL;
	temp2 = (char **)xmalloc(sizeof(char *) * (ft_tablen(allpath) + 2));
	i = -1;
	a = -1;
	while (allpath[++a])
		if (ft_strncmp(cmd, allpath[a], ft_strlen(allpath[a])) == 0)
		{
			temp2[++i] = cmd;
			break;
		}
	del_tab(temp);
	while (allpath[++i] != NULL)
		temp2[i] = x_strjoin(allpath[i], cmd);
	temp2[i] = NULL;
	return (temp2);
}

int	 exec_path(char *cmd, char **path)
{
	int i;

	i = 0;
	while (path[i] != NULL)
	{
		if (access(path[i], X_OK) != -1)
			return (i);
		if (access(path[i], F_OK) != -1 && access(path[i], X_OK) == -1)
		{
			print_err("permission denied", cmd);
			return (-1);
		}
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
	while (i < NB_BUILT)
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
	if (ft_strstr(av->cmd, "./") != NULL && access(av->cmd, F_OK) != -1)
	{
		if  (access(av->cmd, X_OK) == -1)
		{
			print_err("permission denied", av->cmd);
			return (ex);
		}
		ex.type = BIN;
		ex.path = av->cmd;
		ex.argv = av->argv;
	}
	else
	{
		ret = exec_path(av->cmd, path);
		if (ret != -1)
		{
			ex.type = BIN;
			ex.path = path[ret];
			ex.argv = av->argv;
		}
		else
		{
			print_err("command not found", av->cmd);
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
	{
		ex = make_exec_bin(av, *g_env,*l_env);
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
	s = xmalloc(sizeof(char) * len);
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

typedef struct s_multi_redic
{
	char *string;
	int len;
	int fd_to_redir;
	int oldout;
	int tempout;
}						t_multi_redic;

typedef struct s_h
{
	int x;
	int fd[2];
}							t_h;

int get_out(t_redirect ***r, int fd_out)
{
	int i = 0;
	while((*r)[i])i++;
	(*r)[i] = xmalloc(sizeof(t_redirect));
	(*r)[i]->type = 0;
	(*r)[i]->fd_in = 1;
	(*r)[i]->fd_out = fd_out;
	(*r)[i + 1] = NULL;
	return (1);
}

#define READER 0
#define WRITER 1

#define BASIC 0
#define IGNORE 1

#define WRITING 1024

typedef struct s_sf
{
  char  s[WRITING];
  int len;
  struct s_sf *next;
}              t_sf;

typedef struct s_handle_r
{
  int is_redirecting;
  int b_out;
  int fdout[2];
  int b_err;
  int fderr[2];
  t_sf *packets_out;
  t_sf *packets_err;
}           t_handle_r;

void init_handle_redirect(t_redirect **redirect, t_handle_r *hr)
{
  int i;

  //we gonna check if we may redirect
  hr->b_out = 0;
  hr->b_err = 0;
  hr->is_redirecting = 0;
  if (!redirect)
    return ;
  i = 0;
  while (redirect[i])
  {
      if (redirect[i]->type == IGNORE)
        continue;
      if (redirect[i]->fd_in == 1)
        hr->b_out = 1;
      if (redirect[i]->fd_in == 2)
        hr->b_err = 1;
      i++;
  }
  if (hr->b_out)
  {
    if (pipe(hr->fdout) == -1)
      dprintf(2, "failed to pipe\n");
    hr->packets_out = NULL;
  }
  if (hr->b_err)
  {
    pipe(hr->fderr);
    hr->packets_err = NULL;
  }
  hr->is_redirecting = (hr->b_out || hr->b_err) ? 1 : 0;
}

t_sf *create_packet(char *b, int len)
{
    t_sf *tmp;

    tmp = malloc(sizeof(t_sf));
    if (!tmp)
      return (NULL);
    tmp->s[0] = '\0';
    strncpy((char *)tmp->s, b, (size_t)len);
    tmp->len = len;
    tmp->next = NULL;
    return tmp;
}

void father_handle_redirect(t_handle_r *hr)
{
  if (hr->b_out)
  {
      int len;
      t_sf *tmp;
      t_sf *curr;
      char b[WRITING];
      curr = hr->packets_out;
      close(hr->fdout[WRITER]);
      while((len = read(hr->fdout[READER], &b, WRITING)) > 0)
      {
          tmp = create_packet((char *)b, len);
          if (!tmp)
          {
            exit(0);
          }
          if (curr == NULL)
          {
            curr = tmp;
            hr->packets_out = curr;
          }
          else
          {
            curr->next = tmp;
            curr = tmp;
          }
      }
      close(hr->fdout[READER]);
  }
}

/*
void son_handle_redirect(t_handle_r *hr)
{
  if (hr->b_out)
  {
      dup2(hr->fdout[WRITER], STDOUt_sfILENO);
      close(hr->fdout[READER]);
  }
  if (hr->b_err)
  {
    close(hr->fderr[READER]);
    dup2(hr->fderr[WRITER], STDERR_FILENO);
  }
}*/

void active_redirect(t_redirect **r, t_handle_r *hr)
{
  t_sf *p;
  int i;

  i = 0;
	//dprintf(2, "active redirect!!\n");
  while (r[i])
  {
      if (r[i]->type == IGNORE)
        continue;
      if (r[i]->fd_in == STDOUT_FILENO)
      {
        p = hr->packets_out;
        while(p)
        {
          write(r[i]->fd_out, p->s, p->len);
          p = p->next;
        }
      }
      i++;
  }
}
void son_handle_in(int fdin)
{
	//we connect the READER to his STDIN
	dup2(fdin, STDIN_FILENO);
	close(fdin);
}
typedef struct s_pipe
{
	int activate;
	int fds[2];
}t_pipe;

typedef struct	s_executor
{
	t_exec		 ex;
	t_av			 av;
}				t_executor;


int exec_all(t_executor **exs, char **env, int fdin)
{
	pid_t pid;
	int ret;
	int wait_status;
  t_handle_r hr;
	t_pipe p[3] = {{0, {-1,-1}}, {0, {-1,-1}}, {0, {-1, -1}}};

  init_handle_redirect(exs[0]->ex.r, &hr);
	ret = 0;
	if (exs[1] != NULL)
	{
		//dprintf(2, "ACTIVATE THE R %s\n", ex[0]->path);
		p[1].activate = 1;
		pipe(p[1].fds);
	}
	if ((pid = fork()) == -1)
			exit(0);
	if (pid == 0)
	{
      if (fdin != -1)
        son_handle_in(fdin);
			if (hr.is_redirecting)
			{
					close(hr.fdout[READER]);
					dup2(hr.fdout[WRITER], 1);
					close(hr.fdout[WRITER]);
			}
			if (p[1].activate == 1) //tell that we want to redirect the stdout of the son to the pipe
			{
					close(p[1].fds[READER]);
					dup2(p[1].fds[WRITER], 1);
					close(p[1].fds[WRITER]);
			}
			if ((*exs)->ex.type == BASIC)
			{
				execve((*exs)->ex.path, (*exs)->ex.argv, env);
				perror("execve");
			}
			else
					exit((*exs)->ex.fnct((*exs)->av, &g_env, &l_env));
	}
	else
	{
			pid_t pid_bis;
			if (exs[1] != NULL)
			{
				close(p[1].fds[WRITER]);
				exec_all(&exs[1], env, p[1].fds[READER]);
			}
			if (hr.is_redirecting)
				father_handle_redirect(&hr);
			//dprintf(2, "wait... %s\n", exs[0]->ex.path);
			ret = waitpid(-1, &wait_status, WUNTRACED);
			if (WIFSIGNALED(wait_status))
				g_prompt.son = 1;
			if (WIFEXITED(wait_status))
				ret = WEXITSTATUS(wait_status);
			//dprintf(2, "finish to wait!!...%s\n",exs[0]->ex.path);
	}
	//dprintf(2, "preparing to exit... %s\n",exs[0]->ex.path);
  if (hr.is_redirecting)
    active_redirect((*exs)->ex.r, &hr);
	//dprintf(2, "EXIT FATHER  %s with ret: %d\n", exs[0]->ex.path, ret);
	return (ret);
}

void clear_output(t_output *o)
{
	o->len = 0;
	o->string = xmalloc(2);
	if (o->string == NULL)
	{
		//dprintf(2, "xmalloc error.. leaving...\n");
		exit(1);
	}
	o->string[0] = '\0';
}

t_output do_exec(t_executor **exs, int ret,
		t_av av, char *in, int inlen)
{
	t_output o;
	int fdout[2];
	char **env;

	clear_output(&o);
	o.string[0] = '\0';
	o.ret_code = 0;
	int i;
	/*
	dprintf(2, "!!!\n");
	i = 0;
	while(ex && ex[i])
	{
		dprintf(2, "%d test: %s %d \n", i, ex[i]->path, ex[i]->type);
		i++;
	}
	dprintf(2, ">>>%d\n", i);
	*/
	fdout[0] = -1;
	fdout[1] = -1;
	env = convert_env(g_env, l_env);
	if (ret != 0){
		pipe(fdout);
		//we add to the redirection the 1>&pipe
		i = 0;
		while(exs[i])
		{
			if (exs[i + 1] == NULL)
				get_out(&exs[i]->ex.r, fdout[WRITER]);
			i++;
		}
	}
	//set_in(ex.r, (int *)&fdin, in, inlen);
	o.ret_code = 0;
	o.ret_code = exec_all(exs, env, -1);
	if (ret != 0)
	{
		close(fdout[WRITER]);
		char b[WRITING];
		int r;

		//dprintf(2, "RET BABY!!!\n");
		while((r=read(fdout[READER], b, WRITING)) > 0)
		{
			x_strjoins(&o.string, (size_t *)&o.len,(char *)b,r);
		}
		close(fdout[READER]);
	}

	i = -1;
	int y = -1;
	while(exs[++y])
	{
		i = -1;
	while(exs[y]->ex.r[++i])
	{
			if (exs[y]->ex.r[i]->type == IGNORE)
				continue;
			if (ret != 0 && exs[y]->ex.r[i + 1] == NULL)
				continue;
			if (exs[y]->ex.r[i]->fd_out != STDOUT_FILENO || exs[y]->ex.r[i]->fd_out != STDERR_FILENO || exs[y]->ex.r[i]->fd_out != STDIN_FILENO)
				close(exs[y]->ex.r[i]->fd_out);
	}
	}
	return (o);
}

t_output		shell(t_av **av, int ret)
{
	int a;
	t_exec ex;
	t_output all;
	t_output output;
	t_executor **xs;
	int find = -1;
	int xs_i;

	a = -1;
	clear_output(&all);
	clear_output(&output);
	output.string[0] = '\0';
	output.ret_code = 0;
	xs = xmalloc(sizeof(t_executor*) * 2);
	xs_i = 0;
	xs[xs_i] = NULL;
	while (av[++a] != NULL)
	{
		get_alias(&(av[a]));
		// En partant du principe qu'on utilise av->argv et non pas av->arg
		if (av[a]->cmd == NULL && (av[a]->type == TYPE_OR || av[a]->type == TYPE_AND
					|| av[a]->type == TYPE_PIPE || (av[a + 1] != NULL &&
						(av[a + 1]->type == TYPE_AND || av[a + 1]->type == TYPE_OR ||
						 av[a + 1]->type == TYPE_PIPE))))
		{
			ft_dprintf(2, "%s: Invalid null command.\n", NAME);
			clear_output(&output);
			output.ret_code = 127;
			//return (output);
		}
		if (av[a]->argv[0] == NULL)
			continue;
		ex = make_exec(av[a], &g_env, &l_env);
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
		{
			handle_var(KV_SET, "?", "127");
			continue;
		}
		if (av[a + 1] != NULL && av[a + 1]->type == TYPE_PIPE) //HANDLE THE ASYNC MODE
		{
			t_executor **tmp;
			tmp = xmalloc(sizeof(t_executor*) * (xs_i + 2));
			int i = 0;
			while (xs[i])
			{
				tmp[i] = xs[i];
				//dprintf(2, "%s\n", xs[i]->path);
				i++;
			}
			tmp[i] = xmalloc(sizeof(t_executor));
			ft_memcpy(&tmp[i]->ex, (t_exec*)&ex, sizeof(t_exec));
			ft_memcpy(&tmp[i]->av, (t_av*)av[a], sizeof(t_av));
			tmp[i + 1] = NULL;
			xs = tmp;
			xs_i = i + 1;
			continue;
		}
		else if (av[a]->type == TYPE_PIPE)
		{
			xs[xs_i] = xmalloc(sizeof(t_executor));
			ft_memcpy(&xs[xs_i]->ex, (t_exec*)&ex, sizeof(t_exec));
			ft_memcpy(&xs[xs_i]->av, (t_av*)av[a], sizeof(t_av));
			xs[++xs_i] = NULL;
 		}
		else
		{
			xs[xs_i] = xmalloc(sizeof(t_executor));
			ft_memcpy(&xs[0]->ex, (t_exec*)&ex, sizeof(t_exec));
			ft_memcpy(&xs[0]->av, (t_av*)av[a], sizeof(t_av));
			xs[++xs_i] = NULL;
		}
		if (av[a]->type == TYPE_PIPE && output.ret_code != 0)
				continue;
		a_stop(0);
		output = do_exec(xs, ret, *av[a], output.string, output.len);
		if (a_init() == -1)
		{ft_printf("error while getting the set\n");exit(127);}
		//clearing the xs
		xs = xmalloc(sizeof(t_exec*) * 2);
		xs_i = 0;
		xs[xs_i] = NULL;
		char *s;
		handle_var(KV_SET, "?", (s = ft_litoa((unsigned char)output.ret_code)));
		ft_strdel(&s);
		if (ret)
			x_strjoins(&all.string,(size_t *)&all.len,output.string,output.len);
		all.ret_code = output.ret_code;
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
