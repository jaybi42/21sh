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

#define NB_BUILT 12

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
	{"alias", bi_alias},
	{"42info", bi_42info},
	{"glob", bi_glob}
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
void make_r(t_redirect ***r)
{
	int i;

	i = 0;
	while ((*r)[i])
	{
		if ((*r)[i]->type == 0 && (*r)[i]->fd_out == -1 && (*r)[i]->path != NULL)
		{
			(*r)[i]->fd_out = open((*r)[i]->path, (*r)[i]->open_flag, 0666);
			(g_debug) ? ft_dprintf(2, "[?] we did an open(\"%s\")", (*r)[i]->path) : 0;
		}
		else if ((*r)[i]->type == 1 && (*r)[i]->fd == -1 && (*r)[i]->path != NULL)
		{
			(*r)[i]->fd = open((*r)[i]->path, (*r)[i]->open_flag, 0666);
			(g_debug) ? ft_dprintf(2, "[?] we did an open(\"%s\")", (*r)[i]->path) : 0;
		}
		i++;
	}
}

t_exec make_exec(t_av *av, t_list **g_env, t_list **l_env)
{
	t_exec ex;
	ex = make_exec_builtin(av);
	if (ex.type == -1)
	{
		ex = make_exec_bin(av, *g_env,*l_env);
	}
	ex.r = av->redirect;
	//handle_red
	make_r(&ex.r);
	return (ex);
}

/*
   t_output	**exec_tav(t_av **av, int ret)
   {

   }
*/

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

#define BASIC 0
#define IGNORE 1

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
      if (redirect[i]->type == 1)
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

void son_handle_in(int fdin, t_redirect **r)
{
	int i;

	if (fdin != -1)
	{
		dup2(fdin, STDIN_FILENO);
		close(fdin);
	}
	/*
	int fd[2];

	fd[0] = -1;
	fd[0] = -1;
	i = 0;
	if (fdin != -1)
	{
		if (fd[WRITER] == -1 && fd[READER] == -1)
			pipe(fd);
		dup2(fdin, fd[WRITER]);
		close(fdin);
		dprintf(2, "|ou| %d %d\n", fd[WRITER], fd[READER]);
	}
	i = 0;
	while (r[i])
	{
		dprintf(2, "|ou|2 %d %d\n", fd[WRITER], fd[READER]);
		if (fd[WRITER] == -1 && fd[READER] == -1)
			pipe(fd);
		if (r[i]->type == 1 && r[i]->fd != -1)
		{
			dup2(r[i]->fd, fd[WRITER]);
			close(r[i]->fd);
		}
		i++;
	}
	if (fd[WRITER] != -1 && fd[READER] != -1)
	{
		dprintf(2, "|ok|\n");
		close(fd[WRITER]);
		dup2(fd[READER], STDIN_FILENO);
		close(fd[READER]);
	}
	*/
}

typedef struct s_pipe
{
	int activate;
	int fds[2];
}							 t_pipe;

typedef struct	s_executor
{
	t_exec		 ex;
	t_av			 av;
}								t_executor;


int exec_all(t_executor **exs, char **env, int fdin)
{
	pid_t pid;
	int ret;
	int wait_status;
  t_handle_r hr;
	t_pipe p[3] = {{0, {-1,-1}}, {0, {-1,-1}}, {0, {-1, -1}}};

	//if ((*exs)->ex.type == BASIC)
  //	init_handle_redirect(exs[0]->ex.r, &hr);
	ret = 0;
	pid = -1;
	if (exs[1] != NULL)
	{
		p[1].activate = 1;
		pipe(p[1].fds);
	}
	if ((*exs)->ex.type == BASIC && (pid = fork()) == -1)
			exit(0);
	if (pid == 0)
	{
      son_handle_in(fdin, (*exs)->ex.r);
			if (p[1].activate == 1)
			{
					close(p[1].fds[READER]);
					dup2(p[1].fds[WRITER], 1);
					close(p[1].fds[WRITER]);
			}
				execve((*exs)->ex.path, (*exs)->ex.argv, env);
				perror("execve");
				exit(0);
	}
	else if (pid == -1) //BUILTIN HANDLER
	{
		int cpystdout = -1;
		if (exs[1] != NULL)
		{
			cpystdout = dup(1);
			close(1);
			dup2(p[1].fds[WRITER], 1);
			close(p[1].fds[WRITER]);
		}
		(*exs)->ex.fnct((*exs)->av, &g_env, &l_env);
		if (exs[1] != NULL)
		{
			dup2(cpystdout, 1);
			close(cpystdout);
			exec_all(&exs[1], env, p[1].fds[READER]);
		}
	}
	else
	{
			if (fdin != -1)
				close(fdin);
			if (hr.is_redirecting)
				father_handle_redirect(&hr);
			if (exs[1] != NULL)
			{
				close(p[1].fds[WRITER]);
				exec_all(&exs[1], env, p[1].fds[READER]);
			}
			ret = waitpid(-1, &wait_status, WUNTRACED);
			if (WIFEXITED(wait_status))
				ret = WEXITSTATUS(wait_status);
	}
  //if (hr.is_redirecting)
  //  active_redirect((*exs)->ex.r, &hr);
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


t_output do_exec(t_executor **exs, int ret)
{
	t_output o;
	int fdout[2];
	char **env;
	(g_debug) ? ft_dprintf(2, "-- {green}EXEC{eoc} --\n") : 0;
	clear_output(&o);
	o.string[0] = '\0';
	o.ret_code = 0;
	int i;
	fdout[0] = -1;
	fdout[1] = -1;
	env = convert_env(g_env, l_env);
	if (ret != 0)
	{
		pipe(fdout);
		//we add to the redirection the 1>&pipe
		i = 0;
	}
	o.ret_code = 0;
	o.ret_code = exec_all(exs, env, -1);
	if (*g_exit != -1)
			clean_exit(*g_exit);
	if (ret != 0)
	{
		close(fdout[WRITER]);
		char b[WRITING];
		int r;

		//dprintf(2, "RET BABY!!!\n");
		while((r=read(fdout[READER], b, WRITING)) > 0)
			x_strjoins(&o.string, (size_t *)&o.len,(char *)b,r);
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

void clear_stack(t_executor ***stack, int *stack_index)
{
	(*stack) = xmalloc(sizeof(t_exec*) * 2);
	(*stack_index) = 0;
	(*stack[(*stack_index)]) = NULL;
}

void update_stack(t_executor ***p_stack, int *p_stack_index, t_exec *current_ex, t_av *current_av)
{
	(*p_stack)[(*p_stack_index)] = xmalloc(sizeof(t_executor));
	ft_memcpy(&(*p_stack)[(*p_stack_index)]->ex, (t_exec*)current_ex, sizeof(t_exec));
	ft_memcpy(&(*p_stack)[(*p_stack_index)]->av, (t_av*)current_av, sizeof(t_av));
	(*p_stack_index) += 1;
	(*p_stack)[(*p_stack_index)] = NULL;
}

void set_retcode(unsigned char ret_code)
{
	char *s;
	handle_var(KV_SET, "?", (s = ft_litoa(ret_code)));
	ft_strdel(&s);
}

void extend_stack(t_executor ***stack, int *stack_index, t_exec *current_ex, t_av *current_av)
{
	int i;
	t_executor **tmp;

	tmp = xmalloc(sizeof(t_executor*) * ((*stack_index) + 2));
	i = 0;
	while ((*stack)[i])
	{
		tmp[i] = (*stack)[i];
		i++;
	}
	tmp[i] = xmalloc(sizeof(t_executor));
	ft_memcpy(&tmp[i]->ex, (t_exec*)current_ex, sizeof(t_exec));
	ft_memcpy(&tmp[i]->av, (t_av*)current_av, sizeof(t_av));
	tmp[i + 1] = NULL;
	(*stack) = tmp;
	(*stack_index) = i + 1;
}

/*
** struct for handle the shell norme
*/
typedef struct s_shells
{
	int a;
	t_exec ex;
	t_output all;
	t_output output;
	t_executor **stack;
	int					stack_index;
	int find;
}			 	t_shells;

/*
** initialise the shell structure
*/
void shell_init(t_shells *s)
{
	(g_debug) ? ft_dprintf(2, "-- {red}EXECUTION{eoc} --\n") : 0;
	s->find = -1;
	s->a = -1;
	clear_output(&s->all);
	clear_output(&s->output);
	s->output.string[0] = '\0';
	s->output.ret_code = 0;
	s->stack = xmalloc(sizeof(t_executor*) * 2);
	clear_stack(&s->stack, &s->stack_index);
}

/*
** print useful debug
** note: w and v are in the parameters for handle the norme
*/
void shell_print_debug(t_shells *s, int w, int v)
{
	w = 0;
	while ((s->stack)[w])
	{
		if ((s->stack)[w]->ex.type == 0)
			ft_dprintf(2, "%d ; call bin |%s|\n",w, (s->stack)[w]->ex.path);
		else if ((s->stack)[w]->ex.type == 1)
			ft_dprintf(2, "%d ; call builtin |%s|\n",w, (s->stack)[w]->av.cmd);
		v = 0;
		ft_dprintf(2, "[");
		while ((s->stack)[w]->av.argv[v])
		{
				ft_dprintf(2, "'%s'", (s->stack)[w]->av.argv[v]);
				if ((s->stack)[w]->av.argv[v + 1])
					ft_dprintf(2, ", ");
				v++;
		}
		ft_dprintf(2, "]\n");
		w++;
	}
	(g_debug) ? ft_dprintf(2, "{green}----------------------{eoc}\n") : 0;
}

/*
** HANDLE && and ||
*/
int shell_pre_exec_logical_and_or(t_shells *s, t_av **av)
{
	if (av[(s->a)]->type == TYPE_OR || av[(s->a)]->type == TYPE_AND)
	{
		if ((s->find) == 0)
		{
			if (((s->output).ret_code == 0 && av[(s->a)]->type == TYPE_OR) ||
					((s->output).ret_code != 0 && av[(s->a)]->type == TYPE_AND))
			{
				(s->find) = 1;
				return (FALSE);
			}
		}
		else
			return (FALSE);
	}
	if (av[(s->a) + 1] != NULL && (av[(s->a) + 1]->type == TYPE_OR || av[(s->a) + 1]->type == TYPE_AND)
			&& ((s->find) == -1))
		(s->find) = 0;
	else
		(s->find) = -1;
	return (TRUE);
}

/*
** HANDLE |
*/
int shell_pre_exec_logical_pipe(t_shells *s, t_av **av)
{
	if ((s->ex).type == -1)
	{
	set_retcode(127);
	return (FALSE);
	}
	if (av[(s->a) + 1] != NULL && av[(s->a) + 1]->type == TYPE_PIPE)
	{
		extend_stack(&(s->stack), &(s->stack_index),  &(s->ex), av[(s->a)]);
		return (FALSE);
	}
	else
		update_stack(&(s->stack), &(s->stack_index), &(s->ex), av[(s->a)]);
	if (av[(s->a)]->type == TYPE_PIPE && (s->output).ret_code != 0)
			return (FALSE);
	return (TRUE);
}

/*
** SHELL_PRE_EXEC HANDLE THE LOGICAL EXECUTION OF THE COMMAND,
** IT ALSO HANDLE THE open() of the redirection
*/
int shell_pre_exec(t_shells *s, t_av **av)
{
	get_alias(&(av[(s->a)]));
	if (av[(s->a)]->cmd == NULL && (av[(s->a)]->type == TYPE_OR ||
		av[(s->a)]->type == TYPE_AND || av[(s->a)]->type == TYPE_PIPE ||
		(av[(s->a) + 1] != NULL && (av[(s->a) + 1]->type == TYPE_AND ||
		av[(s->a) + 1]->type == TYPE_OR || av[(s->a) + 1]->type == TYPE_PIPE))))
	{
		ft_dprintf(2, "%s: Invalid null command.\n", NAME);
		clear_output(&(s->output));
		(s->output).ret_code = 127;
	}
	if (av[(s->a)]->argv[0] == NULL)
	{
		clear_stack(&(s->stack), &(s->stack_index));
		return (FALSE);
	}
	(s->ex) = make_exec(av[(s->a)], &g_env, &l_env);
	if (!shell_pre_exec_logical_and_or(s, av))
		return (FALSE);
	if (!shell_pre_exec_logical_pipe(s, av))
		return (FALSE);
	return (TRUE);
}

/*
**  SHELL_POST_EXEC handle the cleaning of the executed command
** 	+ set the ret_code ($?)
*/

int shell_post_exec(t_shells *s, int ret)
{
	clear_stack(&(s->stack), &(s->stack_index));
	set_retcode((unsigned char)(s->output).ret_code);
	if (ret)
		x_strjoins(&(s->all).string,(size_t *)&(s->all).len,(s->output).string,
			(s->output).len);
	(s->all).ret_code = (s->output).ret_code;
	return (TRUE);
}

/*
 HOW DOES THIS WORK?

 this is a simple system of stack.

 the stack is extended only when the next command is a pipe.
 It's for handle the async mode.

 for: "ls | cat -e"

 we will have a stack of this sort

 1 | cat -e  |
 0 | ls      |

for: "echo ok ; ls "

 0 | echo ok |
then
 0 | ls      |

etc..
*/
t_output		shell(t_av **av, int ret)
{
	t_shells s;

	shell_init(&s);
	while (av[++(s.a)] != NULL)
	{
		if (!shell_pre_exec(&s, av))
			continue ;
		(g_debug) ? shell_print_debug(&s, 0, 0) : 0;
		a_stop(0);
		s.output = do_exec(s.stack, ret);
		if (a_init() == -1)
		{
			ft_printf("error while getting the set\n");
			exit(127);
		}
		shell_post_exec(&s, ret);
	}
	return (s.all);
}

/*
 ** give him an expr (example: "echo ok | cat -e")
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
