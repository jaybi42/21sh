#include "21sh.h"

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
