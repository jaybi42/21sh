#include "21sh.h"

void impossibru_error(char *s)
{
	ft_dprintf(2, "%s: ERR!: %s\n", NAME, s);
}

void son_handle_redirect(t_handle_r *hr)
{
	if (hr->b_out)
	{
		close(hr->fdout[READER]);
		dup2(hr->fdout[WRITER], 1);
		close(hr->fdout[WRITER]);
	}
	if (hr->b_err)
	{
		close(hr->fderr[READER]);
		dup2(hr->fderr[WRITER], 1);
		close(hr->fderr[WRITER]);
	}
}

/*
** will replace the fd_in
*/

/*
int builtin_dup(int fd, int *pipe)
{
}
*/

/*
void builin_redirect_init(t_handle_r *hr)
{
	if (hr.is_redirecting)
}
*/

void switch_fd_begin(int fd, int *new_fd)
{
	(*new_fd) = dup(fd);
	close(fd);
}

void switch_fd_end(int fd, int *old_fd)
{
	dup2((*old_fd), fd);
	close((*old_fd));
}

void dup_writer_pipe(int fd, int writer)
{
	dup2(writer, fd);
	close(writer);
}

void exec_builtin(t_executor **exs, t_handle_r *hr, char **env)
{
	int cpystdout;
	int cpystderr;

	cpystdout = -1;
	cpystderr = -1;
	if (exs[1] != NULL || (hr->is_redirecting && hr->b_out))
				switch_fd_begin(STDOUT_FILENO, &cpystdout);
	if (exs[1] != NULL)
				dup_writer_pipe(STDOUT_FILENO, hr->p[1].fds[WRITER]);
	else if (hr->is_redirecting && hr->b_out)
			dup_writer_pipe(STDOUT_FILENO, hr->fdout[WRITER]);
	if (hr->is_redirecting && hr->b_err)
	{
			switch_fd_begin(STDERR_FILENO, &cpystderr);
			dup_writer_pipe(STDERR_FILENO, hr->fderr[WRITER]);
	}
	(*exs)->ex.fnct((*exs)->av, &g_env, &l_env);
	if (exs[1] != NULL || (hr->is_redirecting && hr->b_out))
		switch_fd_end(STDOUT_FILENO, &cpystdout);
	if (hr->is_redirecting && hr->b_err)
		switch_fd_end(STDERR_FILENO, &cpystderr);
	if (exs[1] != NULL)
		exec_all(&exs[1], env, hr->p[1].fds[READER]);
	else if (hr->is_redirecting && hr->b_out)
	{
		close(hr->fdout[WRITER]);
		hr->packets_out = read_from_fd(hr->fdout[READER]);
		close(hr->fdout[READER]);
	}
	if (hr->is_redirecting && hr->b_err)
		close(hr->fderr[WRITER]);
}

int exec_all(t_executor **exs, char **env, int fdin)
{
	pid_t pid;
	int ret;
	int wait_status;
  t_handle_r hr;

  init_handle_redirect(exs[0]->ex.r, &hr, (exs[1] != NULL) ? TRUE : FALSE);
	ret = 0;
	pid = -1;
	if ((*exs)->ex.type == BASIC && (pid = fork()) == -1)
			exit(0);
	if (pid == 0)
	{
      son_handle_in(fdin, (*exs)->ex.r);
			if (hr.p[1].activate == 1)
			{
					close(hr.p[1].fds[READER]);
					dup2(hr.p[1].fds[WRITER], 1);
					close(hr.p[1].fds[WRITER]);
			}
			if (hr.is_redirecting)
				son_handle_redirect(&hr);
			execve((*exs)->ex.path, (*exs)->ex.argv, env);
			exit(0);
	}
	else if (pid == -1)
 		exec_builtin(exs, &hr, env);
	else
	{
			if (fdin != -1)
				close(fdin);
			if (hr.is_redirecting)
				father_handle_redirect(&hr);
			if (exs[1] != NULL)
			{
				close(hr.p[1].fds[WRITER]);
				exec_all(&exs[1], env, hr.p[1].fds[READER]);
			}
			ret = waitpid(-1, &wait_status, WUNTRACED);
			if (WIFEXITED(wait_status))
				ret = WEXITSTATUS(wait_status);
	}
  if (hr.is_redirecting)
    active_redirect((*exs)->ex.r, &hr);
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
		i = 0;
		while(exs[i])
		{
			if (exs[i + 1] == NULL)
				get_out(&exs[i]->ex.r, fdout[WRITER]);
			i++;
		}
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

		while ((r=read(fdout[READER], b, WRITING)) > 0)
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
