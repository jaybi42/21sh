#include "21sh.h"

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

  i = 0;
  (void)r;
	if (fdin != -1)
	{
		dup2(fdin, STDIN_FILENO);
		close(fdin);
	}
}

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
